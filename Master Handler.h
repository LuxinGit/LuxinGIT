#pragma once

#include "Canvas.h"
#include "SDL Handler.h"
#include "CLI Handler.h"
#include "Action.h"
#include "Input.h"

struct Master_Handler {

public:

    // NEW
    Action_State        ActionState;
    Canvas_State        CanvasState;
    Cursor_State        CursorState;
    Draw_State          DrawState;
    Input_State         InputState;

    // OLD
    SDL_Handler         SDLHandler;
  /*  Command_Handler     CommandHandler;
    Keyboard_Handler    KeyboardHandler;
    CLI_Handler         CLIHandler;
    Mouse_Handler       MouseHandler;
    GUI_Handler         GUIHandler;*/

    Master_Handler();

    bool runFrame();

    void cleanup() const;

};