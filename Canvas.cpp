#include "Canvas.h"
#include "Master Handler.h"

luxel::luxel() { colour = DEFAULT_BACKGROUND_COLOUR; }
void luxel::resetLuxel() {
    colour = DEFAULT_BACKGROUND_COLOUR;
}

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
    
    void processResetAll(Master_Handler& mh, Command::Command& command) {
        for (auto& l : mh.CanvasState.canvas) l.resetLuxel();
        mh.ActionState.actionQueue = {};
    }
    void processCanvasSize(Master_Handler& mh, Command::Command& command) {
        
        coordinate c = { 0, 0 };
        
        if (command.ID == COMMAND_ID::CANVAS_RESIZE_SET_PAYLOAD) 
            c = std::get<coordinate>(command.args[0]);
        else if (command.ID == COMMAND_ID::CANVAS_RESIZE_SET_HEIGHT)
            c = { 0 , std::get<int>(command.args[0]) };
        else if (command.ID == COMMAND_ID::CANVAS_RESIZE_SET_WIDTH)
            c = { std::get<int>(command.args[0]), 0 };
        
        updateCanvasSize(mh.CanvasState, c);

    }

}