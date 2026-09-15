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

            c.first += s.deltaCursor.first;
            c.second += s.deltaCursor.second;

            return;
        }
        void resetCursors(Cursor_State& s, Canvas_State& canvS) {
            if (!Canvas::newCoordCheck(s.deltaCursor, coordinate{ canvS.width, canvS.height })) {
                s.deltaCursor.first = std::clamp(s.deltaCursor.first, 0.0f, static_cast<float>(canvS.width - 1));
                s.deltaCursor.second = std::clamp(s.deltaCursor.second, 0.0f, static_cast<float>(canvS.height - 1));
            }
            s.cursor = s.deltaCursor;
        }
        bool continuousCheck(bool& base, bool& cont) 
        {
            if (cont)
            {
                cont = false;
                base = false;
                return true;
            }
            else
                return base;
        }
        void setObjectTopLeftFromCursor(Cursor_State& cS, Object::object* o)
        {
            coordinate midPoint{ static_cast<int>(o->width / 2), static_cast<int>(o->height / 2) };
            o->topLeft = coordinate{ cS.cursor } - midPoint;
        }
    }

    void checkCursorData(Application_State& mh) {

        Cursor_State& cS = mh.CursorState;
        Draw_State& dS = mh.DrawState;

        if (continuousCheck(dS.penDown, dS.penContinuous) and cS.cursor != cS.deltaCursor) 
            Draw::drawLineToNewCursor(mh);
        resetCursors(cS, mh.CanvasState);
        if (continuousCheck(cS.carryingObject, cS.carryingContinuous))
            setObjectTopLeftFromCursor(cS, mh.ObjectState.selectedObject);

        
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
            if (d == 2)
                d = ++s.drawStep;
            else
                d = --s.drawStep;
            command.args[0] = 1;
        }

        d = std::clamp(d, DEFAULT_DRAWSTEP_MIN, DEFAULT_DRAWSTEP_MAX);
        
        std::swap(d, s.drawStep);
        command.args = { d };

    }
    void processChangeOrigin(Application_State& mh, Command::Cmd& command) { 
        mh.CursorState.origin = mh.CursorState.cursor;
    } // Probably expand this if I ever want to do stuff with origins, but for now this is fine.
}