#pragma once

#include <array>

#include "Command.h"
#include "CONSTANTS.h"

struct Canvas_Handler;
struct coordinate;

struct Draw_Handler {

    Canvas_Handler& CanvasHandler;
    std::array<uint8_t, 4> drawColour =         DEFAULT_DRAW_COLOUR;
    std::array<uint8_t, 4> backgroundColour =   DEFAULT_BACKGROUND_COLOUR;
    std::array<uint8_t, 4>* activeColour =      &drawColour;
    int pen =                                   DEFAULT_PENWIDTH;

    enum class PEN_MODE {
        DRAW = 0,
        NO_DRAW = 1,
        RUBBER = 2,
        RAINBOW = 3
    };

    PEN_MODE penMode =     static_cast<PEN_MODE>(DEFAULT_PEN_MODE);
    
    bool rainbowMode = false; int pixelsToRainbow = 1000;

    Draw_Handler(Canvas_Handler& CanvH);

    void checkDrawData();

    void drawPoint(const coordinate& c);
    void drawPoint(const coordinate& c, const bool useP);
    void drawLine(const coordinate& origin, const coordinate& destination, const bool useP);
    void drawCircle(const coordinate& c, int radius, const bool fill = false, const bool useP = false);
    void fill(const coordinate& origin, const std::array<uint8_t, 4>& nColour);

    static std::array<uint8_t, 4> getRandomColour();

    void processCommand(const Command& command);

    private:
        void processMetaCommand(const Command& command);
            void processChangePenModeCommand(const Command& command);
                void processRainbowModeCommand(const Command& command);
                void processPenDownCommand(const Command& command);
            void processChangePenWidthCommand(const Command& command);

        void processDrawCommand(const Command& command);
            void processDrawPointCommand(const Command& command);
            void processDrawLineCommand(const Command::DRAW::LINE& setting);
            void processDrawCircleCommand(const Command& command);
            void processChangeColourCommand(const Command& command);
            void processFillCommand(const Command& command);

};
