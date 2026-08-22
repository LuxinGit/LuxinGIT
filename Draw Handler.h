#pragma once

#include <array>

#include "Command.h"

struct Canvas_Handler;

struct Draw_Handler {

    Canvas_Handler& CanvasHandler;
    std::array<uint8_t, 4> colour = { 200, 200, 200, 255 };
    bool rainbowMode = false; int pixelsToRainbow = 1000;
    int pen = 1, penDelta = 1;

    void checkDrawData();

    void drawPoint(const std::pair<float, float>& c);

    void drawLine(std::pair<int, int> origin, std::pair<int, int> destination, const bool useP);

    void drawCircle(const std::pair<float, float>& c, int radius, const bool fill = false);

    void usePen(const std::pair<float, float>& c);

    void drawPoint(const std::pair<float, float>& c, const bool useP);

    Draw_Handler(Canvas_Handler& CanvH);

    static std::array<uint8_t, 4> getRandomColour();

    void processDrawLineCommand(const Command::DRAW::LINE& setting);
    void processDrawCircleCommand(const Command& command);

    void processChangeColourCommand(const Command& command);
    void processRainbowModeCommand(const Command& command);
    void processChangePenWidthCommand(const Command& command);

};
