#pragma once

#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <algorithm>

#include "Command/Command.h"
#include "Application/STRUCTS.h"

struct Application_State;

struct Canvas_State {

    int width                                            =    DEFAULT_CANVAS_WIDTH_CUR;
    int height                                           =   DEFAULT_CANVAS_HEIGHT_CUR;
    std::vector<luxel> displayCanvas{ DEFAULT_CANVAS_SIZE_MAX };
    std::vector<luxel> bufferCanvas { DEFAULT_CANVAS_SIZE_MAX, luxel{ colour{0, 0, 0, 0} } };
    std::vector<luxel>* activeCanvas = &displayCanvas;

};

namespace Canvas {

    bool coordCheck(const Canvas_State& s, const coordinate& c, bool onDisplay = false);
    luxel* getLuxelFromCoord(Canvas_State& s, const coordinate& c, bool onDisplay = false);

    void processCanvasSize(Application_State& mh, Command::Cmd& command);

}