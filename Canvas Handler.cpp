#include "Canvas Handler.h"
#include "Master Handler.h"

luxel::luxel() { colour = DEFAULT_BACKGROUND_COLOUR; }
void luxel::resetLuxel() {
    colour = DEFAULT_BACKGROUND_COLOUR;
}

void Canvas_Handler::updateCanvasSize(int vW, int vH)
{
    width   =    vW;
    height  =    vH;
    MasterHandler.SDLHandler.registerCanvasSizeChange();
}

Canvas_Handler::Canvas_Handler(int& varwidth, int& varheight, Master_Handler& varMH) :
    canvas(DEFAULT_CANVAS_SIZE_MAX),
    width(varwidth), height(varheight),
    MasterHandler(varMH),
    CursorHandler(*this),
    DrawHandler(*this)
{}

coordinate Canvas_Handler::coordinateFromLuxel(const luxel& l)
{
    return coordinateFromIndex(&l - canvas.data(), DEFAULT_CANVAS_WIDTH_MAX);
}
coordinate Canvas_Handler::coordinateFromIndex(const size_t index, const int varW) {
    coordinate c(0, 0);
    c.x = index % varW; c.y = static_cast<int>(index / varW);
    return c;
}

bool Canvas_Handler::coordCheck(const coordinate& c, const size_t width, const size_t height) {
    if (c.x < 0 or static_cast<size_t>(c.x) >= width)   return false;
    if (c.y < 0 or static_cast<size_t>(c.y) >= height)  return false;
    return true;
}
    bool Canvas_Handler::coordCheckDisplay  (const coordinate& c) const {
        return coordCheck(c, width, height);
    } 
    bool Canvas_Handler::coordCheckEntire   (const coordinate& c) const {
        return coordCheck(c, DEFAULT_CANVAS_WIDTH_MAX, DEFAULT_CANVAS_HEIGHT_MAX);
    }

size_t Canvas_Handler::indexFromCoord(const coordinate& c) {
    return size_t(c.y) * DEFAULT_CANVAS_WIDTH_MAX + size_t(c.x);
}

luxel* Canvas_Handler::getLuxelFromCoord(const coordinate& c, bool onDisplay) {
    if (onDisplay) {
        if (!coordCheckDisplay(c)) return nullptr;
        return &canvas[indexFromCoord(c)];
    }
    else if (!coordCheckEntire(c)) return nullptr;
    return &canvas[indexFromCoord(c)];
}

void Canvas_Handler::processCommand(const Command& command) {
    switch (command.type) {
    case Command::TYPE::META:
        processMetaCommand(command);
        break;
    }
}

    void Canvas_Handler::processMetaCommand(const Command& command) {
        using Action = Command::META::ACTION;
        switch (static_cast<Action>(command.action)) {
        case Action::RESET:
            processResetCommand(command);
            break;
        case Action::CANVAS_CHANGE_SIZE:
            processCanvasSizeCommand(command);
            break;
        };
    }
        void Canvas_Handler::processResetCommand(const Command& command) {
            switch (static_cast<Command::META::RESET>(command.setting)) {
            case Command::META::RESET::RESET_ALL:
                for (auto& l : canvas) l.resetLuxel();
                MasterHandler.ActionState.actionQueue = {};
                break;
            case Command::META::RESET::RESET_CURSOR:
                CursorHandler.cursor = DEFAULT_CURSOR_POINT;
                CursorHandler.origin = DEFAULT_CURSOR_POINT;
                CursorHandler.drawStep = DEFAULT_DRAWSTEP_CUR;
                DrawHandler.pen = DEFAULT_PENWIDTH_CUR;
                break;
            }
        }

        void Canvas_Handler::processCanvasSizeCommand(const Command& command) {
            switch (static_cast<Command::META::CANVAS_CHANGE_SIZE>(command.setting)) {
            case Command::META::CANVAS_CHANGE_SIZE::ADD_PAYLOAD:
            {
                coordinate payload = std::get<coordinate>(command.payload);
                updateCanvasSize(width + payload.x, height + payload.y);
                break;
            }
            case Command::META::CANVAS_CHANGE_SIZE::SET_TO_PAYLOAD:
            {
                coordinate payload = std::get<coordinate>(command.payload);
                updateCanvasSize(payload.x, payload.y);
                break;
            }
            case Command::META::CANVAS_CHANGE_SIZE::SET_HEIGHT_TO_PAYLOAD:
            {
                int payload = std::get<int>(command.payload);
                updateCanvasSize(width, payload);
                break;
            }
            case Command::META::CANVAS_CHANGE_SIZE::SET_WIDTH_TO_PAYLOAD:
            {
                int payload = std::get<int>(command.payload);
                updateCanvasSize(payload, height);
                break;
            }

            }
        }
//// NEW

namespace Canvas {
    
    namespace {

        size_t indexFromCoord(const coordinate& c) {
            return size_t(c.y) * DEFAULT_CANVAS_WIDTH_MAX + size_t(c.x);
        }
        void updateCanvasSize(Canvas_State& s, coordinate c)
        {
            if (!coordCheck(s, c, false)) return;
            if(c.x) s.width = c.x;
            if(c.y) s.height = c.y;
            SDL_SetWindowSize(SDL_Handler::Window, s.width, s.height);
        }

    }

    bool coordCheck(const Canvas_State& s, const coordinate& c, bool onDisplay) {
        coordinate tC1 = (onDisplay) ? 
            coordinate(s.width - 1, s.height - 1) : coordinate(DEFAULT_CANVAS_WIDTH_MAX, DEFAULT_CANVAS_HEIGHT_MAX);
        if (c < coordinate(0,0))   return false;
        if (c > tC1)  return false;
        return true;
    }
    luxel* getLuxelFromCoord(Canvas_State& s, const coordinate& c, bool onDisplay) {
        if (!coordCheck(s, c, onDisplay)) return nullptr;
        return &s.canvas[indexFromCoord(c)];
    }
    
    void processResetAll(Master_Handler& mh, Command& command) {
        for (auto& l : mh.CanvasState.canvas) l.resetLuxel();
        mh.ActionState.actionQueue = {};
    }
    void processCanvasSize(Master_Handler& mh, Command& command) {
        
        coordinate c = { 0, 0 };
        
        if (command.ID == COMMAND_ID::CANVAS_RESIZE_SET_PAYLOAD) 
            c = std::get<coordinate>(command.payload);
        else if (command.ID == COMMAND_ID::CANVAS_RESIZE_SET_HEIGHT)
            c = { 0 , std::get<int>(command.payload) };
        else if (command.ID == COMMAND_ID::CANVAS_RESIZE_SET_WIDTH)
            c = { std::get<int>(command.payload), 0 };
        
        updateCanvasSize(mh.CanvasState, c);

    }

}