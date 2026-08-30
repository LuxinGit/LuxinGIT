#pragma once

#include <vector>
#include <array>
#include <string>

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
    coordinate(const long long& varx, const long long& vary) : coordinate(static_cast<int>(varx), static_cast<int>(vary)) {}

};

struct luxel {

    luxel();
    void resetLuxel();

    std::array<uint8_t, 4> colour;

private:
    static uint8_t floatToColour(float f) {
        return static_cast<uint8_t>(f * 255.0f + 0.5f);
    }
    static float colourToFloat(const uint8_t& c) {
        return static_cast<float>(c) / 255;
    }
public:
    static std::array<uint8_t, 4> floatsToColour(const std::array<float,4>& floats) {
        std::array<uint8_t, 4> ret = {};
        for (int i = 0; i < 4; i++) {
            ret[i] = floatToColour(floats[i]);
        }
        return ret;
    }
    static std::array<float, 4> coloursToFloat(const std::array<uint8_t, 4>& colours) {
        std::array<float, 4> ret = {};
        for (int i = 0; i < 4; i++) {
            ret[i] = colourToFloat(colours[i]);
        }
        return ret;
    }
};



// CURSOR HANDLER

static constexpr std::pair<float, float>        DEFAULT_CURSOR_POINT = { 100.0f, 100.0f }; // IF YOU MAKE A CANVAS SMALLER THAN 100, 100 THAT'S ON YOU // ALSO THAT SHOULD STILL BE FINE
static constexpr int                            DEFAULT_DRAWSTEP_MIN = 1;
static constexpr int                            DEFAULT_DRAWSTEP_CUR = 1;
static constexpr int                            DEFAULT_DRAWSTEP_MAX = 50;

// DRAW HANDLER

static constexpr std::array<uint8_t, 4>			DEFAULT_DRAW_COLOUR = { 200, 200, 200, 255 };
static constexpr std::array<uint8_t, 4>			DEFAULT_BACKGROUND_COLOUR = { 255, 255, 255, 255 };
static constexpr int                            DEFAULT_PENWIDTH_MIN = 1;
static constexpr int							DEFAULT_PENWIDTH_CUR = 2;
static constexpr int                            DEFAULT_PENWIDTH_MAX = 50;
static constexpr int							DEFAULT_PEN_MODE = 0; // DRAW

// SDL HANDLER

static constexpr const char*                    DEFAULT_APPLICATION_NAME = "Luxin Paint";

// ENABLES

static constexpr bool							ENABLE_MOUSE = true;
static constexpr bool							ENABLE_CLI = false;

// CANVAS

static constexpr int							DEFAULT_CANVAS_WIDTH = 800;
static constexpr int							DEFAULT_CANVAS_HEIGHT = 600;
