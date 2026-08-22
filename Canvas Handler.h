#pragma once

#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <algorithm>

#include "Command.h"
#include "Cursor Handler.h"
#include "Draw Handler.h"

struct luxel {

    luxel();
    void resetLuxel();

    std::array<uint8_t, 4> colour;

};


struct Canvas_Handler {

    Cursor_Handler CursorHandler;
    Draw_Handler DrawHandler;
    int& width; int& height;
    std::vector<luxel> canvas;

    void clearCanvas();

    size_t indexFromCoord(const std::pair<float, float>& c, const int& w);
    size_t indexFromCoord(const std::pair<float, float>& c);

    bool coordCheck(const std::pair<float, float>& c) const;
    std::pair<float, float> addCoords(const std::pair<float, float>& c1, const std::pair<float, float>& c2);

    luxel* retrieveLuxelFromIndex(const size_t& index);
    luxel* retrieveLuxelFromPoint(const std::pair<float, float>& c, bool coordCheck);
    luxel* retrieveLuxelFromPoint(const std::pair<float, float>& c);

    Canvas_Handler(int& varwidth, int& varheight);

    void refreshCursor();

    void processDrawCommand(const Command& command);
    void processMetaCommand(const Command& command);
    void processMoveCommand(const Command& command);

};
