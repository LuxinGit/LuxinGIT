#include "Master Handler.h"

Master_Handler::Master_Handler(int varwidth, int varheight) :
    width(varwidth), height(varheight),
    CanvasHandler(width, height),
    CommandHandler(*this),
    KeyboardHandler(CommandHandler),
    MouseHandler(CanvasHandler.CursorHandler.deltaCursor, CommandHandler),
    SDLHandler(CanvasHandler),
    CLIHandler(*this) 
{
    if (ENABLE_CLI) CLIHandler.beginCLILoop();
    if (ENABLE_MOUSE) SDL_HideCursor();
};


void Master_Handler::processCommands() {
    KeyboardHandler.harvestKeyboardState();
    MouseHandler.harvestMouseState();
    CommandHandler.processCommands();
    SDLHandler.refreshPresent();
}

void Master_Handler::cleanup() const {
    SDLHandler.cleanup();
}