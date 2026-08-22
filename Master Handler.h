#pragma once

#include "Canvas Handler.h"
#include "SDL Handler.h"
#include "Command Handler.h"
#include "Keyboard Handler.h"
#include "CLI Handler.h"

struct Master_Handler {

private:

    int width; int height;

public:

    Canvas_Handler CanvasHandler;
    SDL_Handler SDLHandler;
    Command_Handler CommandHandler;
    Keyboard_Handler KeyboardHandler;
    CLI_Handler CLIHandler;

    Master_Handler(int varwidth, int varheight);

    int initialiseSDL();

    void processCommands();

    void cleanup() const;

};