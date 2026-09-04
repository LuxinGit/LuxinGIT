#pragma once

#include "Command.h"
#include "CONSTANTS.h"

struct Canvas_State;

struct Cursor_State {

    std::pair<float, float> origin =        DEFAULT_CURSOR_POINT;
    std::pair<float, float> cursor =        DEFAULT_CURSOR_POINT;
    std::pair<float, float> deltaCursor =   DEFAULT_CURSOR_POINT;
    int                     drawStep =      DEFAULT_DRAWSTEP_CUR;
    bool enableCrosshair = true;
};

namespace Cursor {

    void checkCursorData(Application_State& mh);
    
    void processMoveCursor(Application_State& mh, Command::Cmd& command);
    void processChangeDrawstep(Application_State& mh, Command::Cmd& command);
    void processChangeOrigin(Application_State& mh, Command::Cmd& command);
}