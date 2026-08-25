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
    int& width; int& height;
    std::vector<luxel> canvas;

    size_t indexFromCoord(const coordinate& c, const int& w);
    size_t indexFromCoord(const coordinate& c);

    bool coordCheck(const coordinate& c) const;

    luxel* getLuxelFromIndex(const size_t& index);
    luxel* getLuxelFromCoord(const coordinate& c, bool coordCheck);
    luxel* getLuxelFromCoord(const coordinate& c);

    Canvas_Handler(int& varwidth, int& varheight, Master_Handler& varMH);

    void processCommand(const Command& command);

private:
        void processMetaCommand(const Command& command);
            void processResetCommand(const Command& command);

};
