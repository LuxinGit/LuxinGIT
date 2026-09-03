#include "Master Handler.h"
#include "COMMAND REPO.h"

Master_Handler::Master_Handler() :
    SDLHandler(*this)
{
    Input::initialiseBindings(InputState);
    Input::GUI::initialiseGUI(*this);
};


bool Master_Handler::runFrame() {
    if (!Input::harvestApplicationInputs(*this)) return false;
    Command::Processor::processCommands(*this);
    SDLHandler.renderFrame();
    return true;
}

void Master_Handler::cleanup() const {
    SDLHandler.cleanup();
}
