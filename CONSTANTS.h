#pragma once

#include <vector>
#include <array>

// CURSOR HANDLER

static constexpr std::pair<float, float>        DEFAULT_CURSOR_POINT = { 100, 100 }; // IF YOU MAKE A CANVAS SMALLER THAN 100, 100 THAT'S ON YOU // ALSO THAT SHOULD STILL BE FINE
static constexpr int                            DEFAULT_DRAWSTEP = 1;
static constexpr int                            DEFAULT_CROSSHAIR_RADIUS = 5;

// DRAW HANDLER

static constexpr std::array<uint8_t, 4>			DEFAULT_DRAW_COLOUR = { 200, 200, 200, 255 };
static constexpr std::array<uint8_t, 4>			DEFAULT_BACKGROUND_COLOUR = { 255, 255, 255, 255 };
static constexpr int							DEFAULT_PENWIDTH = 2;

// ENABLES

static constexpr bool							ENABLE_MOUSE = true;
static constexpr bool							ENABLE_CLI = false;

// CANVAS

static constexpr int							DEFAULT_CANVAS_WIDTH = 800;
static constexpr int							DEFAULT_CANVAS_HEIGHT = 600;
