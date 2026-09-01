#pragma once

#include "Canvas.h"
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

    // NEW
    Action_State        ActionState;
    Canvas_State        CanvasState;
    Cursor_State        CursorState;
    Draw_State          DrawState;

    // OLD
    SDL_Handler         SDLHandler;
    Command_Handler     CommandHandler;
    Keyboard_Handler    KeyboardHandler;
    CLI_Handler         CLIHandler;
    Mouse_Handler       MouseHandler;
    GUI_Handler         GUIHandler;

    Master_Handler();

    void renderFrame();

    void cleanup() const;

};