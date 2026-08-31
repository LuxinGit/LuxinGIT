#pragma once

#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <algorithm>

#include "COMMAND.h"
#include "Cursor Handler.h"
#include "Draw Handler.h"

struct Master_Handler;

struct Canvas_Handler {

    Master_Handler& MasterHandler;
    Cursor_Handler CursorHandler;
    Draw_Handler DrawHandler;
    int& width; 
    int& height;
    std::vector<luxel> canvas;

    size_t indexFromCoord(const coordinate& c);

    static bool coordCheck(const coordinate& c, const size_t width, const size_t height);
        bool coordCheckDisplay(const coordinate& c) const;
        bool coordCheckEntire(const coordinate& c) const;

    luxel* getLuxelFromCoord(const coordinate& c, bool onDisplay = false);

    coordinate coordinateFromLuxel(const luxel&);
    static coordinate coordinateFromIndex(const size_t index, const int varW);

    void updateCanvasSize(int width, int height);
    Canvas_Handler(int& varwidth, int& varheight, Master_Handler& varMH);

    void processCommand(const Command& command);

private:
        void processMetaCommand(const Command& command);
            void processResetCommand(const Command& command);
            void processCanvasSizeCommand(const Command& command);

};
