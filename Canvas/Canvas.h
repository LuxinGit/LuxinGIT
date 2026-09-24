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

    bool newCoordCheck(
        const coordinate& c,
        const coordinate& max = coordinate{ DEFAULT_CANVAS_WIDTH_MAX, DEFAULT_CANVAS_HEIGHT_MAX });
    luxel* newGetLuxelFromCoord(
        std::vector<luxel>& v, 
        const coordinate& c,
        const coordinate& dim = coordinate{ DEFAULT_CANVAS_WIDTH_MAX, DEFAULT_CANVAS_HEIGHT_MAX} // default case = checking against canvas dimensions
        );
    luxel* getDisplayedLuxelFromActiveCanvas(Canvas_State& cS, const coordinate& c);

    void swapActiveCanvas(Canvas_State&); // used to swap between displayCanvas and canvasBuffer.

    void processCanvasSize(Application_State& mh, Command::Cmmd& command);

}