#pragma once

#include "Canvas.h"
#include "SDLWrapper.h"
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
    SDL_State           SDLState;

    Master_Handler();

    bool runFrame();

    void cleanup();

};