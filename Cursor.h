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

    void checkCursorData(Master_Handler& mh);
    
    void processMoveCursor(Master_Handler& mh, Command::Cmd& command);
    void processChangeDrawstep(Master_Handler& mh, Command::Cmd& command);
    void processChangeOrigin(Master_Handler& mh, Command::Cmd& command);
}