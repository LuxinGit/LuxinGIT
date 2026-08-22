#include "Master Handler.h"

Master_Handler::Master_Handler(int varwidth, int varheight) :
    width(varwidth), height(varheight),
    CanvasHandler(width, height),
    CommandHandler(*this),
    KeyboardHandler(CommandHandler),
    SDLHandler(CanvasHandler),
    CLIHandler(*this) {};

int Master_Handler::initialiseSDL() {
    return (SDLHandler.initialiseSDL());
}

void Master_Handler::processCommands() {
    KeyboardHandler.harvestKeyboardState();
    CommandHandler.processCommands();
    SDLHandler.refreshPresent();
}

void Master_Handler::cleanup() const {
    SDLHandler.cleanup();
}