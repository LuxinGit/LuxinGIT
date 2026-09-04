#pragma once

#include "Canvas/Canvas.h"
#include "Canvas/Cursor.h"
#include "Canvas/Draw.h"
#include "Platform/SDLWrapper.h"
#include "Command/Action.h"
#include "Command/Input.h"

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