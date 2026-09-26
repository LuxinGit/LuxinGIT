#include <algorithm>

#include "Cursor.h"
#include "Command Definition.h"
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

	const colour retrieveColourUnderCursor(const Application_State& s)
	{
		return Canvas::getColourFromCoordinate(coordinate{ s.CursorState.deltaCursor }, *s.CanvasState.activeCanvas, s.CanvasState.width);
	}

    void processMoveCursor(Application_State& mh, Command::Cmmd& command) {
        
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

    void processChangeDrawstep(Application_State& aS, Command::Cmmd& command) {
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
    void processChangeOrigin(Application_State& mh, Command::Cmmd& command) { 
        mh.CursorState.origin = mh.CursorState.cursor;
    } // Probably expand this if I ever want to do stuff with origins, but for now this is fine.
}

namespace Cursor::Move
{
	namespace Dir::Interpreter
	{
		static coordinate getDeltaFromDirection(const Command::argmap& aMap)
		{
			return Direction::get(std::get<std::string>(aMap.at("Direction")));
		}

		static const int* getDistanceFromDirection(const Command::argmap& aMap)
		{
			return std::get_if<int>(&aMap.at("Distance"));
		}

		static coordinate calculateDestinationFromDirection(const Cursor_State& s, const Command::argmap& aMap)
		{
			coordinate delta = getDeltaFromDirection(aMap);
			const int* distptr = getDistanceFromDirection(aMap);

			int distance = s.drawStep;
			if (distptr) distance = *distptr;

			return delta * distance + coordinate{ s.deltaCursor };
		}

		static std::vector<Command::cmd> interpretDirectionalCursorMove(const Application_State& s, const Command::cmd& c)
		{
			coordinate destination =
				calculateDestinationFromDirection(s.CursorState, c.args);

			Command::cmd newC{ &::Cursor::Move::Set::CURSOR_MOVE_SET };
			newC.setArg("Destination", destination);

			return { std::move(newC) };
		}
	}

	namespace Dir::Validator
	{
		static bool validateDirection(const Command::argument& a, const Command::argmd& md)
		{
			return Direction::dirmap.contains(std::get<std::string>(a));
		}
	}

	namespace Dir
	{
		const Command::dfn CURSOR_MOVE_DIR =
		{
			.name = "cursor_move_direction",

			.argDefinitions =
			{
				{
					"Direction",
					Command::argmd
					{
						.type = Command::Argument::ARGTYPE::STRING,
						.defaultValue = std::monostate(),
						.validator = &Validator::validateDirection,
						.required = true,
					}
				},

				{
					"Distance",
					Command::argmd
					{
						.type = Command::Argument::ARGTYPE::INT,
						.defaultValue = std::monostate(),
						.validator = nullptr,
						.required = false
					}
				}
			},

			.interp = &Interpreter::interpretDirectionalCursorMove
		};
	}

	namespace Set::Processor
	{
		static void processSet(Application_State& s, Command::cmd& c)
		{
			coordinate& nc = std::get<coordinate>(c.args.at("Destination"));
			std::pair<float, float> ncff = static_cast<std::pair<float, float>>(nc);

			std::swap(ncff, s.CursorState.deltaCursor);
			nc = coordinate(ncff);
		}
	}

	namespace Set
	{
		const Command::dfn CURSOR_MOVE_SET =
		{
			.name = "cursor_move_set",

			.argDefinitions =
			{
				{
					"Destination",
					Command::argmd
					{
						.type = Command::Argument::ARGTYPE::COORDINATE,
						.defaultValue = std::monostate(),
						.validator = nullptr,
						.required = true
					}
				}
			},

			.prcssr = &Processor::processSet
		};
	}
}