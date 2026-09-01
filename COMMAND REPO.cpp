#include "COMMAND REPO.h"
#include "Action.h"
#include "Canvas Handler.h"

Command_Repo COMMAND_REPO = { {

    // MOVEMENT
    {
        Command{COMMAND_ID::MOVE_UP, Command::MOVE::DIRECTION::UP, true},
        COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
        SDL_SCANCODE_W,
        std::nullopt,
        "up",
        std::nullopt
    },
    {
        Command{COMMAND_ID::MOVE_DOWN, Command::MOVE::DIRECTION::DOWN, true},
        COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
        SDL_SCANCODE_S,
        std::nullopt,
        "down",
        std::nullopt
    },
    {
        Command{COMMAND_ID::MOVE_LEFT, Command::MOVE::DIRECTION::LEFT, true},
        COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
        SDL_SCANCODE_A,
        std::nullopt,
        "left",
        std::nullopt
    },
    {
        Command{COMMAND_ID::MOVE_RIGHT, Command::MOVE::DIRECTION::RIGHT, true},
        COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
        SDL_SCANCODE_D,
        std::nullopt,
        "right",
        std::nullopt
    },
    {
        Command{COMMAND_ID::MOVE_RESET, Command::MOVE::SET::RESET_TO_ORIGIN, false},
        COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
        SDL_SCANCODE_Q,
        std::nullopt,
        "reset",
        std::nullopt
    },
    {
        Command{COMMAND_ID::MOVE_SET_POINT, Command::MOVE::SET::USE_PAYLOAD, false, DEFAULT_CURSOR_POINT},
        COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
        std::nullopt,
        std::nullopt,
        "move",
        std::nullopt
    },
    {
        Command{COMMAND_ID::MOVE_SAVE_ORIGIN, Command::META::SAVE_ORIGIN::NORMAL, false},
        COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
        SDL_SCANCODE_2,
        std::nullopt,
        "save_origin",
        std::nullopt
    },

    // DRAW
    {
        Command{COMMAND_ID::DRAW_CIRCLE, Command::DRAW::CIRCLE::NORMAL, false},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        SDL_SCANCODE_G,
        std::nullopt,
        "circle",
        std::nullopt
    },
    {
        Command{COMMAND_ID::DRAW_CIRCLE_RAINBOW, Command::DRAW::CIRCLE::RAINBOW, false},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        SDL_SCANCODE_L,
        std::nullopt,
        "rainbow_circle",
        std::nullopt
    },
    {
        Command{COMMAND_ID::DRAW_FILL_PAYLOAD, Command::DRAW::FILL::USE_PAYLOAD, false},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        std::nullopt,
        std::nullopt,
        "fill",
        std::nullopt
    },
    {
        Command{COMMAND_ID::DRAW_FILL_DRAWCOLOUR, Command::DRAW::FILL::USE_DRAW_COLOUR, false},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        SDL_SCANCODE_F,
        std::nullopt,
        std::nullopt,
        std::nullopt
    },

    // COLOUR
    {
        Command{COMMAND_ID::COLOUR_RESET, Command::META::CHANGE_COLOUR::DEFAULT, false},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        std::nullopt,
        std::nullopt,
        "default_colour",
        std::nullopt
    },
    {
        Command{COMMAND_ID::COLOUR_RANDOM, Command::META::CHANGE_COLOUR::RANDOM, false},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        SDL_SCANCODE_J,
        std::nullopt,
        "random_colour",
        std::nullopt
    },
    {
        Command{COMMAND_ID::COLOUR_SET_DRAW, Command::META::CHANGE_COLOUR::USE_PAYLOAD_DRAW, false, DEFAULT_DRAW_COLOUR},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        std::nullopt,
        std::nullopt,
        "colour_draw",
        GUI_METADATA{
            GUI_METADATA::HEADER::TOOLS,
            "Set draw colour",
            GUI_METADATA::COLOUR_METADATA{
                GUI_METADATA::COLOUR_METADATA::resolveDrawColourChange
            }
        }
    },
    {
        Command{COMMAND_ID::COLOUR_SET_PICK, Command::META::CHANGE_COLOUR::USE_HOVERED_LUXEL, false, DEFAULT_DRAW_COLOUR},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        std::nullopt,
        SDL_BUTTON_MIDDLE,
        std::nullopt,
        std::nullopt,
    },
    {
        Command{COMMAND_ID::COLOUR_SET_BACKGROUND, Command::META::CHANGE_COLOUR::USE_PAYLOAD_BACKGROUND, false, DEFAULT_DRAW_COLOUR},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        std::nullopt,
        std::nullopt,
        "colour_background",
        GUI_METADATA{
            GUI_METADATA::HEADER::TOOLS,
            "Set background colour",
            GUI_METADATA::COLOUR_METADATA{
                GUI_METADATA::COLOUR_METADATA::resolveBackgroundColourChange
            }
        }
    },

    // PEN MODE
    {
        Command{COMMAND_ID::PENMODE_DRAW, Command::META::CHANGE_PENMODE::DRAW, false},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        SDL_SCANCODE_Y,
        std::nullopt,
        "penmode_draw",
        GUI_METADATA{
            GUI_METADATA::HEADER::TOOLS,
            "Draw"
        }
    },
    {
        Command{COMMAND_ID::PENMODE_RUBBER, Command::META::CHANGE_PENMODE::RUBBER, false},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        SDL_SCANCODE_U,
        std::nullopt,
        "penmode_rubber",
        GUI_METADATA{
            GUI_METADATA::HEADER::TOOLS,
            "Rubber"
        }
    },
    {
        Command{COMMAND_ID::PENMODE_RAINBOW, Command::META::CHANGE_PENMODE::RAINBOW, false},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        SDL_SCANCODE_E,
        std::nullopt,
        "penmode_rainbow",
        GUI_METADATA{
            GUI_METADATA::HEADER::TOOLS,
            "Rainbow"
        }
    },

    // PEN
    {
        Command{COMMAND_ID::PEN_DOWN, Command::META::CHANGE_PENMODE::PEN_DOWN, false, 0},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        SDL_SCANCODE_1,
        std::nullopt,
        "pen",
        std::nullopt
    },
    {
        Command{COMMAND_ID::PEN_HELD_DOWN, Command::META::CHANGE_PENMODE::PEN_DOWN, true, 1},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        std::nullopt,
        SDL_BUTTON_LMASK,
        std::nullopt,
        std::nullopt
    },

    // DRAW STEP
    {
        Command{COMMAND_ID::DRAWSTEP_DECREASE, Command::META::CHANGE_DRAWSTEP::ADD_PAYLOAD, false, -1},
        COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
        SDL_SCANCODE_Z,
        std::nullopt,
        "step_down",
        std::nullopt
    },
    {
        Command{COMMAND_ID::DRAWSTEP_INCREASE, Command::META::CHANGE_DRAWSTEP::ADD_PAYLOAD, false, 1},
        COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
        SDL_SCANCODE_X,
        std::nullopt,
        "step_up",
        std::nullopt
    },
    {
        Command{COMMAND_ID::DRAWSTEP_SET, Command::META::CHANGE_DRAWSTEP::SET_TO_PAYLOAD, false, 1},
        COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
        std::nullopt,
        std::nullopt,
        "step_set",
        GUI_METADATA{
            GUI_METADATA::HEADER::TOOLS,
            "Change Drawstep",
            GUI_METADATA::SLIDER_METADATA{
                DEFAULT_DRAWSTEP_MIN,
                DEFAULT_DRAWSTEP_MAX,
                GUI_METADATA::SLIDER_METADATA::resolveDrawstep
            }
        }
    },

    // PEN WIDTH
    {
        Command{COMMAND_ID::PEN_WIDTH_DECREASE, Command::META::CHANGE_PEN_WIDTH::ADD_PAYLOAD, false, -1},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        SDL_SCANCODE_V,
        std::nullopt,
        "pen_thinner",
        std::nullopt
    },
    {
        Command{COMMAND_ID::PEN_WIDTH_INCREASE, Command::META::CHANGE_PEN_WIDTH::ADD_PAYLOAD, false, 1},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        SDL_SCANCODE_B,
        std::nullopt,
        "pen_thicker",
        std::nullopt
    },
    {
        Command{COMMAND_ID::PEN_SET, Command::META::CHANGE_PEN_WIDTH::SET_TO_PAYLOAD, false, 1},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        std::nullopt,
        std::nullopt,
        "pen_set",
        GUI_METADATA{
            GUI_METADATA::HEADER::TOOLS,
            "Change Pen Width",
            GUI_METADATA::SLIDER_METADATA{
                DEFAULT_PENWIDTH_MIN,
                DEFAULT_PENWIDTH_MAX,
                GUI_METADATA::SLIDER_METADATA::resolvePenWidth
            }
        }
    },

    // INPUT
    {
        Command{COMMAND_ID::INPUT_CLI_ENABLE, Command::APP::INPUT_MODE::CLI, false},
        COMMAND_PROCESSOR_ID::CLI_HANDLER,
        SDL_SCANCODE_0,
        std::nullopt,
        std::nullopt,
        std::nullopt
    },
    {
        Command{COMMAND_ID::INPUT_MOUSE_ENABLE, Command::APP::INPUT_MODE::MOUSE, false},
        COMMAND_PROCESSOR_ID::MOUSE_HANDLER,
        SDL_SCANCODE_3,
        std::nullopt,
        std::nullopt,
        std::nullopt
    },

    // RESET
    {
        Command{COMMAND_ID::RESET_CANVAS, Command::META::RESET::RESET_CANVAS, false},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        SDL_SCANCODE_C,
        std::nullopt,
        "reset_canvas",
        GUI_METADATA{
            GUI_METADATA::HEADER::FILE,
            "Reset Canvas"
        }
    },
    {
        Command{COMMAND_ID::RESET_ACTION_QUEUE, Command::META::RESET::RESET_ACTION_QUEUE, false},
        COMMAND_PROCESSOR_ID::ACTION_HANDLER,
        std::nullopt,
        std::nullopt,
        "reset_action_queue",
        std::nullopt,
        &Action::processClearActionQueue
    },
    {
        Command{COMMAND_ID::RESET_ALL, Command::META::RESET::RESET_ALL, false},
        COMMAND_PROCESSOR_ID::CANVAS_HANDLER,
        SDL_SCANCODE_R,
        std::nullopt,
        "reset_all",
        GUI_METADATA{
            GUI_METADATA::HEADER::FILE,
            "Reset All (Includes undo/redo history)"
        },
        &Canvas::processResetAll
    },
    {
        Command{COMMAND_ID::RESET_CURSOR, Command::META::RESET::RESET_CURSOR, false},
        COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
        SDL_SCANCODE_9,
        std::nullopt,
        "reset_cursor",
        std::nullopt
    },

    // UNDO / REDO
    {
        Command{COMMAND_ID::UNDO, Command::APP::UNDO_REDO::UNDO, false},
        COMMAND_PROCESSOR_ID::ACTION_HANDLER,
        SDL_SCANCODE_N,
        std::nullopt,
        "undo",
        GUI_METADATA{
            GUI_METADATA::HEADER::EDIT,
            "Undo"
        },
        &Action::processUndo
    },
    {
        Command{COMMAND_ID::REDO, Command::APP::UNDO_REDO::REDO, false},
        COMMAND_PROCESSOR_ID::ACTION_HANDLER,
        SDL_SCANCODE_M,
        std::nullopt,
        "redo",
        GUI_METADATA{
            GUI_METADATA::HEADER::EDIT,
            "Redo"
        },
        &Action::processRedo
    },
    {
        Command{COMMAND_ID::CANVAS_RESIZE_SET_PAYLOAD, Command::META::CANVAS_CHANGE_SIZE::SET_TO_PAYLOAD, false, coordinate{400, 400}},
        COMMAND_PROCESSOR_ID::CANVAS_HANDLER,
        SDL_SCANCODE_7,
        std::nullopt,
        "resize",
        std::nullopt,
        &Canvas::processCanvasSize
    },
    {
        Command{COMMAND_ID::CANVAS_RESIZE_SET_HEIGHT, Command::META::CANVAS_CHANGE_SIZE::SET_HEIGHT_TO_PAYLOAD, false, 400},
        COMMAND_PROCESSOR_ID::CANVAS_HANDLER,
        std::nullopt,
        std::nullopt,
        "resize_height",
        GUI_METADATA{
            GUI_METADATA::HEADER::FILE,
            "Adjust canvas height",
            GUI_METADATA::SLIDER_METADATA{
                DEFAULT_CANVAS_HEIGHT_MIN,
                DEFAULT_CANVAS_HEIGHT_MAX,
                GUI_METADATA::SLIDER_METADATA::resolveCanvasHeight
            }
        },
        &Canvas::processCanvasSize
    },
    {
        Command{COMMAND_ID::CANVAS_RESIZE_SET_WIDTH, Command::META::CANVAS_CHANGE_SIZE::SET_WIDTH_TO_PAYLOAD, false, 400},
        COMMAND_PROCESSOR_ID::CANVAS_HANDLER,
        std::nullopt,
        std::nullopt,
        "resize_width",
        GUI_METADATA{
            GUI_METADATA::HEADER::FILE,
            "Adjust canvas width",
            GUI_METADATA::SLIDER_METADATA{
                DEFAULT_CANVAS_WIDTH_MIN,
                DEFAULT_CANVAS_WIDTH_MAX,
                GUI_METADATA::SLIDER_METADATA::resolveCanvasWidth
            }
        },
        &Canvas::processCanvasSize
    },

    } };