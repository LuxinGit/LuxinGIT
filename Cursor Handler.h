#pragma once

#include "Command.h"

struct Canvas_Handler;

static constexpr std::pair<float, float>        DEFAULT_CURSOR_POINT = { 100, 100 }; // IF YOU MAKE A CANVAS SMALLER THAN 100, 100 THAT'S ON YOU // ALSO THAT SHOULD STILL BE FINE
static constexpr int                            DEFAULT_DRAWSTEP = 1;
static constexpr int                            DEFAULT_CROSSHAIR_RADIUS = 5;

struct Cursor_Handler {

    Cursor_Handler(Canvas_Handler& varCanvH);

    Canvas_Handler& CanvasHandler;

    std::pair<float, float> origin =            DEFAULT_CURSOR_POINT;
    std::pair<float, float> cursor =            DEFAULT_CURSOR_POINT;
    std::pair<float, float> deltaCursor =       { {}, {} };
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