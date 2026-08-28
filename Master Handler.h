#pragma once

#include "Canvas Handler.h"
#include "SDL Handler.h"
#include "Command Handler.h"
#include "Keyboard Handler.h"
#include "CLI Handler.h"
#include "Mouse Handler.h"
#include "Action Handler.h"
#include "GUI Handler.h"

struct Master_Handler {

private:

    int width; int height;

    void initialiseBindings();

public:

    Canvas_Handler      CanvasHandler;
    SDL_Handler         SDLHandler;
    Command_Handler     CommandHandler;
    Keyboard_Handler    KeyboardHandler;
    CLI_Handler         CLIHandler;
    Mouse_Handler       MouseHandler;
    Action_Handler      ActionHandler;
    GUI_Handler         GUIHandler;

    Master_Handler(int varwidth, int varheight);

    void renderFrame();

    void cleanup() const;

};