#include "Master Handler.h"

Master_Handler::Master_Handler(int varwidth, int varheight) :
    width(varwidth), height(varheight),
    CanvasHandler(width, height),
    CommandHandler(*this),
    KeyboardHandler(CommandHandler),
    CLIHandler(*this) {};

int Master_Handler::initialiseSDL() {
    return (SDLHandler.initialiseSDL(width, height, CanvasHandler.canvas));
}

void Master_Handler::processCommands() {
    KeyboardHandler.harvestKeyboardState();
    CommandHandler.processCommands();
}

void Master_Handler::cleanup() const {
    SDLHandler.cleanup();
}