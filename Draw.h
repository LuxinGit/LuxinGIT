#pragma once

#include <array>

#include "COMMAND.h"
#include "CONSTANTS.h"

struct Draw_State {

    colour  drawColour       =  DEFAULT_DRAW_COLOUR;
    colour  backgroundColour =  DEFAULT_BACKGROUND_COLOUR;
    colour* activeColour     =  &drawColour;
    
    int pen =                                   DEFAULT_PENWIDTH_CUR;
    bool penDown =                              !ENABLE_MOUSE;
    bool penContinuous =                        false;
    enum class PEN_MODE {
        DRAW,
        RUBBER,
        RAINBOW
    };
    PEN_MODE penMode = static_cast<PEN_MODE>    (DEFAULT_PEN_MODE);

    size_t pixelsDrawn = 0;
    int pixelsToRainbow = 1000;

};

namespace Draw {

    void drawLineToNewCursor(Master_Handler& mh);

    void processChangePenWidth(Master_Handler& mh, Command::Command& command);
    void processCircle(Master_Handler& mh, Command::Command& command);
    void processFill(Master_Handler& mh, Command::Command& command);
    void processPenDown(Master_Handler& mh, Command::Command& command);
    void processChangePenMode(Master_Handler& mh, Command::Command& command);
    void processChangeColour(Master_Handler& mh, Command::Command& command);
    void processClearCanvas(Master_Handler& mh, Command::Command& command);

}
