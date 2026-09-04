#include "Application.h"
#include "COMMAND REPO.h"

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



