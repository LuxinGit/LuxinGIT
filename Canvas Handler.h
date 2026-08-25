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


struct coordinate {
    int x;
    int y;

    bool operator==(const coordinate& other) const {
        return x == other.x and y == other.y;
    }

    coordinate operator+(const coordinate& other) const {
        return { x + other.x, y + other.y };
    }

    coordinate operator-(const coordinate& other) const {
        return { x - other.x, y - other.y };
    }

    coordinate(const int& varx, const int& vary) : x(varx), y(vary) {}
    coordinate(const std::pair<int, int>& varC) : x(varC.first), y(varC.second) {}
    coordinate(const float& varfx, const float& varfy) : x(static_cast<int>(varfx)), y(static_cast<int>(varfy)) {}
    coordinate(const std::pair<float, float>& varfC) : coordinate(varfC.first, varfC.second) {}

};



struct Canvas_Handler {

    Cursor_Handler CursorHandler;
    Draw_Handler DrawHandler;
    int& width; int& height;
    std::vector<luxel> canvas;

    void clearCanvas();

    size_t indexFromCoord(const coordinate& c, const int& w);
    size_t indexFromCoord(const coordinate& c);

    bool coordCheck(const coordinate& c) const;

    luxel* getLuxelFromIndex(const size_t& index);
    luxel* getLuxelFromCoord(const coordinate& c, bool coordCheck);
    luxel* getLuxelFromCoord(const coordinate& c);

    Canvas_Handler(int& varwidth, int& varheight);

    void processCommand(const Command& command);

private:
        void processMetaCommand(const Command& command);
            void processResetCommand(const Command& command);

};
