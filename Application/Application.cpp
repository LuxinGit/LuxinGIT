#include "Application.h"

void LuxinPaint::initialiseApplicationState(Application_State& s)
{
    Input::initialiseBindings(s.InputState);
    SDL::initialiseSDL(s.SDLState, s.CanvasState);
    Input::GUI::initialiseGUI(s);
}

bool LuxinPaint::runFrame(Application_State& s)
{
    if (!Input::harvestApplicationInputs(s)) return false;
    Command::Processor::processCommands(s);
    SDL::renderFrame(s);
    return true;
}

void LuxinPaint::cleanup(Application_State& s)
{
    Input::GUI::cleanupGUI();
    SDL::cleanup(s.SDLState);
}

void LuxinPaint::processReset(Application_State& s, Command::Cmd& command)
// 0: INT ActionQueue
// 1: INT Canvas
// 2: INT Cursor
// 3: INT Colours
{
    Command::Cmd c = { COMMAND_ID::INVALID };

    bool resetActions = std::get<int>(command.args[0]);

    if (resetActions)
        Action::processClearActionQueue(s, command);

    if (std::get<int>(command.args[1]))
        Draw::processClearCanvas(s, !resetActions);

    if (std::get<int>(command.args[2])) {
        c.ID = COMMAND_ID::CURSOR_MOVE;
        c.args = { 1, coordinate{ DEFAULT_CURSOR_POINT } };
        Cursor::processMoveCursor(s, c);
    }

    if (std::get<int>(command.args[3])) {
        c.ID = COMMAND_ID::COLOUR;
        c.args = { 1, 0, DEFAULT_DRAW_COLOUR };
        Draw::processChangeColour(s, c);
        c.args = { 2, 0, DEFAULT_BACKGROUND_COLOUR };
        Draw::processChangeColour(s, c);
    }
}
 