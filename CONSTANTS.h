#pragma once

#include <vector>
#include <array>

struct coordinate {
    int x = 0;
    int y = 0;

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


// CURSOR HANDLER

static constexpr std::pair<float, float>        DEFAULT_CURSOR_POINT = { 100.0f, 100.0f }; // IF YOU MAKE A CANVAS SMALLER THAN 100, 100 THAT'S ON YOU // ALSO THAT SHOULD STILL BE FINE
static constexpr int                            DEFAULT_DRAWSTEP = 1;
static constexpr int                            DEFAULT_CROSSHAIR_RADIUS = 5; // DEPRECIATED - CURSOR RADIUS IS TAKING PENWIDTH AS COMMAND

// DRAW HANDLER

static constexpr std::array<uint8_t, 4>			DEFAULT_DRAW_COLOUR = { 200, 200, 200, 255 };
static constexpr std::array<uint8_t, 4>			DEFAULT_BACKGROUND_COLOUR = { 255, 255, 255, 255 };
static constexpr int							DEFAULT_PENWIDTH = 2;
static constexpr int							DEFAULT_PEN_MODE = 0; // DRAW

// ENABLES

static constexpr bool							ENABLE_MOUSE = true;
static constexpr bool							ENABLE_CLI = false;

// CANVAS

static constexpr int							DEFAULT_CANVAS_WIDTH = 800;
static constexpr int							DEFAULT_CANVAS_HEIGHT = 600;
