#pragma once

#include "Command.h"

struct Canvas_Handler;

static constexpr std::pair<float, float>        DEFAULT_CURSOR_POINT = { 100, 100 }; // IF YOU MAKE A CANVAS SMALLER THAN 100, 100 THAT'S ON YOU // ALSO THAT SHOULD STILL BE FINE
static constexpr int                            DEFAULT_DRAWSTEP = 1;

struct Cursor_Handler {

    Cursor_Handler(Canvas_Handler& varCanvH);

    Canvas_Handler& CanvasHandler;

    std::pair<float, float> origin =            DEFAULT_CURSOR_POINT;
    std::pair<float, float> cursor =            DEFAULT_CURSOR_POINT;
    std::pair<float, float> deltaCursor =       { {}, {} };
    int                     drawStep =          DEFAULT_DRAWSTEP;
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