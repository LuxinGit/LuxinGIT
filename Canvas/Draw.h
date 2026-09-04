#pragma once

#include "Command/Command.h"
#include "Application/STRUCTS.h"

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

    void drawLineToNewCursor(Application_State& mh);

    void processChangePenWidth(Application_State& mh, Command::Cmd& command);
    void processCircle(Application_State& mh, Command::Cmd& command);
    void processFill(Application_State& mh, Command::Cmd& command);
    void processPenDown(Application_State& mh, Command::Cmd& command);
    void processChangePenMode(Application_State& mh, Command::Cmd& command);
    void processChangeColour(Application_State& mh, Command::Cmd& command);
    void processClearCanvas(Application_State& mh, Command::Cmd& command);

}
