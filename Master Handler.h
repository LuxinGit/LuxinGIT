#pragma once

#include "Canvas Handler.h"
#include "SDL Handler.h"
#include "Command Handler.h"
#include "Keyboard Handler.h"
#include "CLI Handler.h"
#include "Mouse Handler.h"
#include "Action.h"
#include "GUI Handler.h"

struct Master_Handler {

private:

    void initialiseBindings();

public:

    int width; int height;

    // OLD
    Canvas_Handler      CanvasHandler;
    SDL_Handler         SDLHandler;
    Command_Handler     CommandHandler;
    Keyboard_Handler    KeyboardHandler;
    CLI_Handler         CLIHandler;
    Mouse_Handler       MouseHandler;
    GUI_Handler         GUIHandler;

    // NEW
    Action_State        ActionState;
    Canvas_State        CanvasState;

    Master_Handler(int varwidth, int varheight);

    void renderFrame();

    void cleanup() const;

};