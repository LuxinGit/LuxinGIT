#pragma once

#include "Command.h"
#include "CONSTANTS.h"

struct Canvas_Handler;

struct Cursor_Handler {

    Cursor_Handler(Canvas_Handler& varCanvH);

    Canvas_Handler& CanvasHandler;

    std::pair<float, float> origin =            DEFAULT_CURSOR_POINT;
    std::pair<float, float> cursor =            DEFAULT_CURSOR_POINT;
    std::pair<float, float> deltaCursor =       DEFAULT_CURSOR_POINT;
    int                     drawStep =          DEFAULT_DRAWSTEP;
    bool penDown = true;
    bool enableCrosshair = true;
    int crosshairRadius =                       DEFAULT_CROSSHAIR_RADIUS;
    size_t pixelsDrawn = 0;

    static void resetPoint(float& f, const int& width);
    void resetCursors();

    void refreshCursor();

    void processChangeDrawstepCommand(const Command& command);
    void processDirectionCommand(const Command& command);
    void processSetCommand(const Command& command);
    void processMoveCommand(const Command& command);


};