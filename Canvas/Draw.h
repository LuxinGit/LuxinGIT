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

    void processChangePenWidth(Application_State& mh, Command::Cmmd& command);
    void processCircle(Application_State&, Command::Cmmd& command);
    void processFill(Application_State& mh, Command::Cmmd& command);
    void processPenDown(Application_State& mh, Command::Cmmd& command);
    void processChangePenMode(Application_State& mh, Command::Cmmd& command);
    void processChangeColour(Application_State& mh, Command::Cmmd& command);
    void processClearCanvas(Application_State& mh, bool full);
}


namespace Draw::Circle { extern const Command::dfn DRAW_CIRCLE; }
namespace Draw::Fill   { extern const Command::dfn DRAW_FILL; }

namespace Draw::Pen::Colour::SetSource { extern const Command::dfn DRAW_COLOUR_SETSOURCE; }
namespace Draw::Pen::Colour::SetColour { extern const Command::dfn DRAW_COLOUR_SETCOLOUR; }

namespace Draw::Pen::Mode::SetMode { extern const Command::dfn DRAW_PEN_MODE_SETMODE; }
