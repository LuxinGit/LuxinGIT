#include "Canvas Handler.h"
#include "Master Handler.h"

luxel::luxel() { colour = DEFAULT_BACKGROUND_COLOUR; }
void luxel::resetLuxel() {
    colour = DEFAULT_BACKGROUND_COLOUR;
}

coordinate Canvas_Handler::coordinateFromLuxel(const luxel& l)
{
    return coordinateFromIndex(&l - canvas.data(), width);
}

coordinate Canvas_Handler::coordinateFromIndex(const size_t index, const int varW) {
    coordinate c(0, 0);
    c.x = index % varW; c.y = static_cast<int>(index / varW);
    return c;
}
void Canvas_Handler::updateCanvasSize(int vW, int vH)
{
    vW = std::clamp(vW, DEFAULT_CANVAS_WIDTH_MIN, DEFAULT_CANVAS_WIDTH_MAX);
    vH = std::clamp(vH, DEFAULT_CANVAS_HEIGHT_MIN, DEFAULT_CANVAS_HEIGHT_MAX);
    std::vector<luxel> ret(vW * vH);
    for (int i = 0; i < canvas.size(); i++) {
        coordinate curC = coordinateFromIndex(i, width);
        if (curC.x >= vW or curC.y >= vH) continue;
        ret[indexFromCoord(curC, vW)] = canvas[i];
    }

    canvas  =    std::move(ret);
    width   =    vW;
    height  =    vH;

    MasterHandler.SDLHandler.registerCanvasSizeChange();
}

Canvas_Handler::Canvas_Handler(int& varwidth, int& varheight, Master_Handler& varMH) :
    width(varwidth), height(varheight),
    MasterHandler(varMH),
    CursorHandler(*this),
    DrawHandler(*this),
    canvas(std::vector<luxel>(width* height)) {
    // Only concern here is that if underlying height / width changes then this will need to be recalculated.
}

size_t Canvas_Handler::indexFromCoord(const coordinate& c, const int& w) {
    // ASSUMES POSITIVE X/Y. INDEXING WITH THIS INDEX WITHOUT SIZE CHECKING MAY CAUSE OUT OF BOUNDARY MEMORY CRASH [IF COORD > LAST LUXEL INDEX].
    return size_t(c.y) * w + size_t(c.x);
}
size_t Canvas_Handler::indexFromCoord(const coordinate& c)  {
    return indexFromCoord(c, width);
}

bool Canvas_Handler::coordCheck(const coordinate& c) const {
    if (c.x < 0 or c.x >= width) return false;
    if (c.y < 0 or c.y >= height) return false;
    return true;
} 

luxel* Canvas_Handler::getLuxelFromIndex(const size_t& index) { return &canvas[index]; }
luxel* Canvas_Handler::getLuxelFromCoord(const coordinate& c, bool coordCheck) { return &canvas[indexFromCoord(c)]; }
luxel* Canvas_Handler::getLuxelFromCoord(const coordinate& c) { return (coordCheck(c) ? getLuxelFromCoord(c, true) : nullptr); }


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
                MasterHandler.ActionHandler.resetActionQueue();
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
