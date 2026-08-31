#pragma once

#include <vector>
#include <array>
#include <string>

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

static constexpr bool							ENABLE_MOUSE =					true;
static constexpr bool							ENABLE_CLI =					false;

// CANVAS

static constexpr int							DEFAULT_CANVAS_WIDTH_MIN =		100;
static constexpr int							DEFAULT_CANVAS_WIDTH_CUR =		800;
static constexpr int							DEFAULT_CANVAS_WIDTH_MAX =		4096;
static constexpr int							DEFAULT_CANVAS_HEIGHT_MIN =		100;
static constexpr int							DEFAULT_CANVAS_HEIGHT_CUR =		600;
static constexpr int							DEFAULT_CANVAS_HEIGHT_MAX =		4096;
static constexpr int							DEFAULT_CANVAS_SIZE_MAX = DEFAULT_CANVAS_WIDTH_MAX * DEFAULT_CANVAS_HEIGHT_MAX;