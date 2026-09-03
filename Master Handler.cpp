#include "Master Handler.h"
#include "COMMAND REPO.h"

Master_Handler::Master_Handler() :
    //CommandHandler(*this),
    //KeyboardHandler(*this),
    //MouseHandler(CursorState.deltaCursor, *this),
    SDLHandler(*this),
    //CLIHandler(*this),
    //GUIHandler(*this)
{
    initialiseBindings();
    //if (ENABLE_CLI) CLIHandler.beginCLILoop();
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

void Master_Handler::initialiseBindings() {
    Input::initialiseBindings(InputState);
}