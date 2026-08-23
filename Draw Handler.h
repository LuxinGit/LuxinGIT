#pragma once

#include <array>

#include "Command.h"
#include "CONSTANTS.h"

struct Canvas_Handler;

struct Draw_Handler {

    Canvas_Handler& CanvasHandler;
    std::array<uint8_t, 4> colour =     DEFAULT_DRAW_COLOUR;
    int pen =                           DEFAULT_PENWIDTH;
    
    bool rainbowMode = false; int pixelsToRainbow = 1000;

    Draw_Handler(Canvas_Handler& CanvH);

    void checkDrawData();

    void drawPoint(const std::pair<float, float>& c);
    void drawPoint(const std::pair<float, float>& c, const bool useP);

    void drawLine(std::pair<int, int> origin, std::pair<int, int> destination, const bool useP);
    void drawCircle(const std::pair<float, float>& c, int radius, const bool fill = false, const bool useP = false);

    void fill(const std::pair<int, int>& origin, const std::array<uint8_t, 4>& nColour);

    static std::array<uint8_t, 4> getRandomColour();

    void processDrawPointCommand(const Command& command);
    void processDrawLineCommand(const Command::DRAW::LINE& setting);
    void processDrawCircleCommand(const Command& command);
    void processChangeColourCommand(const Command& command);
    void processRainbowModeCommand(const Command& command);
    void processChangePenWidthCommand(const Command& command);

    void processFillCommand(const Command& command);
    void processDrawCommand(const Command& command);

};
