#pragma once

#include "Command/Command.h"
#include "Application/STRUCTS.h"

struct Canvas_State;

struct Cursor_State {

    std::pair<float, float> origin =        DEFAULT_CURSOR_POINT;
    std::pair<float, float> cursor =        DEFAULT_CURSOR_POINT;
    std::pair<float, float> deltaCursor =   DEFAULT_CURSOR_POINT;
    int                     drawStep =      DEFAULT_DRAWSTEP_CUR;
    bool enableCrosshair = true;
    bool carryingObject = false;
    bool carryingContinuous = false;
};

namespace Cursor
{
    void checkCursorData(Application_State& mh);

    const colour retrieveColourUnderCursor(const Application_State& s);

    void processMoveCursor(Application_State& mh, Command::Cmmd& command);
    void processChangeDrawstep(Application_State& mh, Command::Cmmd& command);
    void processChangeOrigin(Application_State& mh, Command::Cmmd& command);
}

namespace Cursor::Move::Dir
{
    extern const Command::dfn CURSOR_MOVE_DIR;
}

namespace Cursor::Move::Set
{
    extern const Command::dfn CURSOR_MOVE_SET;
}