#pragma once

#include "Command.h"

struct Canvas_Handler;

struct Cursor_Handler {

    Cursor_Handler(Canvas_Handler& varCanvH);

    Canvas_Handler& CanvasHandler;

    std::pair<float, float> origin = { 100, 100 };
    std::pair<float, float> cursor = { 100, 100 };
    std::pair<float, float> deltaCursor = { 0, 0 };

    int drawStep = 1, drawStepDelta = 1;
    bool penDown = true;
    size_t pixelsDrawn = 0;

    static void resetPoint(float& f, const int& width);
    void resetCursors();

    void checkCursorLine();

    void processChangeDrawstepCommand(const Command& command);
    void processDirectionCommand(const Command& command);
    void processSetCommand(const Command& command);
    void processMoveCommand(const Command& command);


};