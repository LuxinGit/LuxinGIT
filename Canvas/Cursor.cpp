#include <algorithm>

#include "Cursor.h"
#include "Application/Application.h"

#include <cassert>

namespace Cursor {

    namespace {
        void convertDirectionToSet(Cursor_State& s, std::pair<float, float>& c) {
            
            float d = static_cast<float>(s.drawStep);        
            c.first *= d;
            c.second *= d;

            return;
        }
        void resetCursors(Cursor_State& s, Canvas_State& canvS) {
            if (!Canvas::coordCheck(canvS, s.deltaCursor, true)) {
                s.deltaCursor.first = std::clamp(s.deltaCursor.first, 0.0f, static_cast<float>(canvS.width - 1));
                s.deltaCursor.second = std::clamp(s.deltaCursor.second, 0.0f, static_cast<float>(canvS.height - 1));
            }
            s.cursor = s.deltaCursor;
        }
    }

    void checkCursorData(Application_State& mh) {
        if (mh.DrawState.penDown and mh.CursorState.cursor != mh.CursorState.deltaCursor) 
            Draw::drawLineToNewCursor(mh);
        if (mh.DrawState.penContinuous) { mh.DrawState.penContinuous = false; mh.DrawState.penDown = false; }
        resetCursors(mh.CursorState, mh.CanvasState);
    }

    void processMoveCursor(Application_State& mh, Command::Cmd& command) {
        
        // 0:SETTING [DIR/SET/ORI], 1:COORDINATE [DRAWSTEP_USAGE/DESTINATION]
        
        Cursor_State& s = mh.CursorState;
        int setting = std::get<int>(command.args[0]);
        std::pair<float, float> c = std::get<coordinate>(command.args[1]);

        if (setting == 0) {
            convertDirectionToSet(s, c);
        }
        else if (setting == 2) {
            c = s.origin;
        }

        std::swap(s.deltaCursor, c);
        command.ID = COMMAND_ID::CURSOR_MOVE;
        command.args = { 1, c };

    }

    void processChangeDrawstep(Application_State& aS, Command::Cmd& command) {
        // 0:INT Setting {ADD,SET}, 1:INT Delta

        Cursor_State& s = aS.CursorState;
        int& d = std::get<int>(command.args[1]);

        if (std::get<int>(command.args[0]) == 0) {
            command.args[0] = 1;
            d += s.drawStep;
        }

        d = std::clamp(d, DEFAULT_DRAWSTEP_MIN, DEFAULT_DRAWSTEP_MAX);
        
        std::swap(d, s.drawStep);
        command.args = { d };

    }
    void processChangeOrigin(Application_State& mh, Command::Cmd& command) { 
        mh.CursorState.origin = mh.CursorState.cursor;
    } // Probably expand this if I ever want to do stuf with origins, but for now this is fine.
}