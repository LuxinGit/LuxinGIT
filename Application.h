#pragma once

#include "Canvas.h"
#include "SDLWrapper.h"
#include "CLI Handler.h"
#include "Action.h"
#include "Input.h"

struct Application_State {

public:

    // NEW
    Action_State        ActionState;
    Canvas_State        CanvasState;
    Cursor_State        CursorState;
    Draw_State          DrawState;
    Input_State         InputState;
    SDL_State           SDLState;

};

namespace LuxinPaint {

    void initialiseApplicationState(Application_State&);

    bool runFrame(Application_State&);

    void cleanup(Application_State&);

}