#include <algorithm>

#include "Cursor.h"
#include "Canvas.h"
#include "Master Handler.h"

namespace Cursor {

    namespace {
        std::pair<float,float> convertDirectionToSet(Cursor_State& s, Command::Cmd& command) {
            
            float d = static_cast<float>(s.drawStep);
            
            switch (command.ID) {
            case COMMAND_ID::MOVE_UP:
                return { s.deltaCursor.first, s.deltaCursor.second - d };
            case COMMAND_ID::MOVE_RIGHT:
                return { s.deltaCursor.first + d, s.deltaCursor.second };
            case COMMAND_ID::MOVE_DOWN:
                return { s.deltaCursor.first, s.deltaCursor.second + d };
            case COMMAND_ID::MOVE_LEFT:
                return { s.deltaCursor.first - d, s.deltaCursor.second };
            }
            assert(false); // stupidass
            return { 0.0f, 0.0f };
        }
        void resetCursors(Cursor_State& s, Canvas_State& canvS) {
            if (!Canvas::coordCheck(canvS, s.deltaCursor, true)) {
                s.deltaCursor.first = std::clamp(s.deltaCursor.first, 0.0f, static_cast<float>(canvS.width - 1));
                s.deltaCursor.second = std::clamp(s.deltaCursor.second, 0.0f, static_cast<float>(canvS.width - 1));
            }
            s.cursor = s.deltaCursor;
        }
    }

    void checkCursorData(Master_Handler& mh) {
        if (mh.DrawState.penDown and mh.CursorState.cursor != mh.CursorState.deltaCursor) 
            Draw::drawLineToNewCursor(mh);
        if (mh.DrawState.penContinuous) { mh.DrawState.penContinuous = false; mh.DrawState.penDown = false; }
        resetCursors(mh.CursorState, mh.CanvasState);
    }

    void processMoveCursor(Master_Handler& mh, Command::Cmd& command) {
        
        Cursor_State& s = mh.CursorState;
        std::pair<float, float> c = s.deltaCursor;

        if (command.ID == COMMAND_ID::MOVE_SET_POINT)
            c = std::get<coordinate>(command.args[0]);
        else if (command.ID == COMMAND_ID::RESET_CURSOR)
            c = s.origin;
        else c = convertDirectionToSet(s, command);

        std::swap(s.deltaCursor, c);
        command.ID = COMMAND_ID::MOVE_SET_POINT;
        command.args = { c };

    }
    void processChangeDrawstep(Master_Handler& mh, Command::Cmd& command) {
        
        Cursor_State& s = mh.CursorState;
        int d = s.drawStep;
        int p = std::get<int>(command.args[0]);

        if (command.ID == COMMAND_ID::DRAWSTEP_SET)
            d = p;
        else { // DRAWSTEP_INCREASE OR DRAWSTEP_DECREASE
            d = std::clamp(d + p, DEFAULT_DRAWSTEP_MIN, DEFAULT_DRAWSTEP_MAX);
        }

        std::swap(d, s.drawStep);
        command.ID = COMMAND_ID::DRAWSTEP_SET;
        command.args = { d };

    }
    void processChangeOrigin(Master_Handler& mh, Command::Cmd& command) { 
        mh.CursorState.origin = mh.CursorState.cursor;
    } // Probably expand this if I ever want to do stuf with origins, but for now this is fine.
}