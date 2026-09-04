#include "Master Handler.h"
#include "COMMAND REPO.h"

Master_Handler::Master_Handler()
{
    Input::initialiseBindings(InputState);
    SDL::initialiseSDL(SDLState, CanvasState);
    Input::GUI::initialiseGUI(*this);

};


bool Master_Handler::runFrame() {
    if (!Input::harvestApplicationInputs(*this)) return false;
    Command::Processor::processCommands(*this);
    SDL::renderFrame(*this);
    return true;
}

void Master_Handler::cleanup() {
    Input::GUI::cleanupGUI();
    SDL::cleanup(SDLState);
}
