#include "COMMAND REPO.h"
#include "Action.h"
#include "Canvas.h"


Command_Repo COMMAND_REPO = { {

    //// MOVEMENT
    //{
    //    Command_OLD{COMMAND_ID::MOVE_UP, Command_OLD::MOVE::DIRECTION::UP, true},
    //    COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
    //    SDL_SCANCODE_W,
    //    std::nullopt,
    //    "up",
    //    std::nullopt,
    //    &Cursor::processMoveCursor
    //},
    //{
    //    Command_OLD{COMMAND_ID::MOVE_DOWN, Command_OLD::MOVE::DIRECTION::DOWN, true},
    //    COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
    //    SDL_SCANCODE_S,
    //    std::nullopt,
    //    "down",
    //    std::nullopt,
    //    &Cursor::processMoveCursor
    //},
    //{
    //    Command_OLD{COMMAND_ID::MOVE_LEFT, Command_OLD::MOVE::DIRECTION::LEFT, true},
    //    COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
    //    SDL_SCANCODE_A,
    //    std::nullopt,
    //    "left",
    //    std::nullopt,
    //    &Cursor::processMoveCursor
    //},
    //{
    //    Command_OLD{COMMAND_ID::MOVE_RIGHT, Command_OLD::MOVE::DIRECTION::RIGHT, true},
    //    COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
    //    SDL_SCANCODE_D,
    //    std::nullopt,
    //    "right",
    //    std::nullopt,
    //    &Cursor::processMoveCursor
    //},
    //{
    //    Command_OLD{COMMAND_ID::MOVE_RESET, Command_OLD::MOVE::SET::RESET_TO_ORIGIN, false},
    //    COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
    //    SDL_SCANCODE_Q,
    //    std::nullopt,
    //    "reset",
    //    std::nullopt,
    //    &Cursor::processMoveCursor
    //},
    //{
    //    Command_OLD{COMMAND_ID::MOVE_SET_POINT, Command_OLD::MOVE::SET::USE_PAYLOAD, false, DEFAULT_CURSOR_POINT},
    //    COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
    //    std::nullopt,
    //    std::nullopt,
    //    "move",
    //    std::nullopt,
    //    &Cursor::processMoveCursor
    //},
    //{
    //    Command_OLD{COMMAND_ID::MOVE_SAVE_ORIGIN, Command_OLD::META::SAVE_ORIGIN::NORMAL, false},
    //    COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
    //    SDL_SCANCODE_2,
    //    std::nullopt,
    //    "save_origin",
    //    std::nullopt,
    //    &Cursor::processChangeOrigin
    //},

    //// DRAW
    //{
    //    Command_OLD{COMMAND_ID::DRAW_CIRCLE, Command_OLD::DRAW::CIRCLE::NORMAL, false},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    SDL_SCANCODE_G,
    //    std::nullopt,
    //    "circle",
    //    std::nullopt,
    //    &Draw::processCircle
    //},
    //{
    //    Command_OLD{COMMAND_ID::DRAW_FILL_PAYLOAD, Command_OLD::DRAW::FILL::USE_PAYLOAD, false},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    std::nullopt,
    //    std::nullopt,
    //    "fill",
    //    std::nullopt,
    //    &Draw::processFill
    //},
    //{
    //    Command_OLD{COMMAND_ID::DRAW_FILL_DRAWCOLOUR, Command_OLD::DRAW::FILL::USE_DRAW_COLOUR, false},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    SDL_SCANCODE_F,
    //    std::nullopt,
    //    std::nullopt,
    //    std::nullopt,
    //    &Draw::processFill
    //},

    //// COLOUR
    //{
    //    Command_OLD{COMMAND_ID::COLOUR_SET_DEFAULT, Command_OLD::META::CHANGE_COLOUR::DEFAULT, false},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    std::nullopt,
    //    std::nullopt,
    //    "default_colour",
    //    std::nullopt,
    //    &Draw::processChangeColour
    //},
    //{
    //    Command_OLD{COMMAND_ID::COLOUR_SET_RANDOM, Command_OLD::META::CHANGE_COLOUR::RANDOM, false},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    SDL_SCANCODE_J,
    //    std::nullopt,
    //    "random_colour",
    //    std::nullopt,
    //    &Draw::processChangeColour
    //},
    //{
    //    Command_OLD{COMMAND_ID::COLOUR_SET_DRAW, Command_OLD::META::CHANGE_COLOUR::USE_PAYLOAD_DRAW, false, DEFAULT_DRAW_COLOUR},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    std::nullopt,
    //    std::nullopt,
    //    "colour_draw",
    //    GUI_METADATA{
    //        GUI_METADATA::HEADER::TOOLS,
    //        "Set draw colour",
    //        GUI_METADATA::COLOUR_METADATA{
    //            GUI_METADATA::COLOUR_METADATA::resolveDrawColourChange
    //        }
    //    },
    //    &Draw::processChangeColour
    //},
    //{
    //    Command_OLD{COMMAND_ID::COLOUR_SET_PICK, Command_OLD::META::CHANGE_COLOUR::USE_HOVERED_LUXEL, false, DEFAULT_DRAW_COLOUR},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    std::nullopt,
    //    SDL_BUTTON_MIDDLE,
    //    std::nullopt,
    //    std::nullopt,
    //    &Draw::processChangeColour
    //},
    //{
    //    Command_OLD{COMMAND_ID::COLOUR_SET_BACKGROUND, Command_OLD::META::CHANGE_COLOUR::USE_PAYLOAD_BACKGROUND, false, DEFAULT_DRAW_COLOUR},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    std::nullopt,
    //    std::nullopt,
    //    "colour_background",
    //    GUI_METADATA{
    //        GUI_METADATA::HEADER::TOOLS,
    //        "Set background colour",
    //        GUI_METADATA::COLOUR_METADATA{
    //            GUI_METADATA::COLOUR_METADATA::resolveBackgroundColourChange
    //        }
    //    },
    //    &Draw::processChangeColour
    //},

    //// PEN MODE
    //{
    //    Command_OLD{COMMAND_ID::PENMODE_DRAW, Command_OLD::META::CHANGE_PENMODE::DRAW, false},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    SDL_SCANCODE_Y,
    //    std::nullopt,
    //    "penmode_draw",
    //    GUI_METADATA{
    //        GUI_METADATA::HEADER::TOOLS,
    //        "Draw"
    //    },
    //    &Draw::processChangePenMode
    //},
    //{
    //    Command_OLD{COMMAND_ID::PENMODE_RUBBER, Command_OLD::META::CHANGE_PENMODE::RUBBER, false},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    SDL_SCANCODE_U,
    //    std::nullopt,
    //    "penmode_rubber",
    //    GUI_METADATA{
    //        GUI_METADATA::HEADER::TOOLS,
    //        "Rubber"
    //    },
    //    &Draw::processChangePenMode
    //},
    //{
    //    Command_OLD{COMMAND_ID::PENMODE_RAINBOW, Command_OLD::META::CHANGE_PENMODE::RAINBOW, false},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    SDL_SCANCODE_E,
    //    std::nullopt,
    //    "penmode_rainbow",
    //    GUI_METADATA{
    //        GUI_METADATA::HEADER::TOOLS,
    //        "Rainbow"
    //    },
    //    &Draw::processChangePenMode
    //},

    //// PEN
    //{
    //    Command_OLD{COMMAND_ID::PEN_DOWN, Command_OLD::META::CHANGE_PENMODE::PEN_DOWN, false, 0},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    SDL_SCANCODE_1,
    //    std::nullopt,
    //    "pen",
    //    std::nullopt,
    //    &Draw::processPenDown
    //},
    //{
    //    Command_OLD{COMMAND_ID::PEN_HELD_DOWN, Command_OLD::META::CHANGE_PENMODE::PEN_DOWN, true, 1},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    std::nullopt,
    //    SDL_BUTTON_LMASK,
    //    std::nullopt,
    //    std::nullopt,
    //    &Draw::processPenDown
    //},

    //// DRAW STEP
    //{
    //    Command_OLD{COMMAND_ID::DRAWSTEP_DECREASE, Command_OLD::META::CHANGE_DRAWSTEP::ADD_PAYLOAD, false, -1},
    //    COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
    //    SDL_SCANCODE_Z,
    //    std::nullopt,
    //    "step_down",
    //    std::nullopt,
    //    &Cursor::processChangeDrawstep
    //},
    //{
    //    Command_OLD{COMMAND_ID::DRAWSTEP_INCREASE, Command_OLD::META::CHANGE_DRAWSTEP::ADD_PAYLOAD, false, 1},
    //    COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
    //    SDL_SCANCODE_X,
    //    std::nullopt,
    //    "step_up",
    //    std::nullopt,
    //    &Cursor::processChangeDrawstep
    //},
    //{
    //    Command_OLD{COMMAND_ID::DRAWSTEP_SET, Command_OLD::META::CHANGE_DRAWSTEP::SET_TO_PAYLOAD, false, 1},
    //    COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
    //    std::nullopt,
    //    std::nullopt,
    //    "step_set",
    //    GUI_METADATA{
    //        GUI_METADATA::HEADER::TOOLS,
    //        "Change Drawstep",
    //        GUI_METADATA::SLIDER_METADATA{
    //            DEFAULT_DRAWSTEP_MIN,
    //            DEFAULT_DRAWSTEP_MAX,
    //            GUI_METADATA::SLIDER_METADATA::resolveDrawstep
    //        }
    //    },
    //    &Cursor::processChangeDrawstep
    //},

    //// PEN WIDTH
    //{
    //    Command_OLD{COMMAND_ID::PEN_WIDTH_DECREASE, Command_OLD::META::CHANGE_PEN_WIDTH::ADD_PAYLOAD, false, -1},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    SDL_SCANCODE_V,
    //    std::nullopt,
    //    "pen_thinner",
    //    std::nullopt,
    //    &Draw::processChangePenMode
    //},
    //{
    //    Command_OLD{COMMAND_ID::PEN_WIDTH_INCREASE, Command_OLD::META::CHANGE_PEN_WIDTH::ADD_PAYLOAD, false, 1},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    SDL_SCANCODE_B,
    //    std::nullopt,
    //    "pen_thicker",
    //    std::nullopt,
    //    &Draw::processChangePenMode
    //},
    //{
    //    Command_OLD{COMMAND_ID::PEN_SET, Command_OLD::META::CHANGE_PEN_WIDTH::SET_TO_PAYLOAD, false, 1},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    std::nullopt,
    //    std::nullopt,
    //    "pen_set",
    //    GUI_METADATA{
    //        GUI_METADATA::HEADER::TOOLS,
    //        "Change Pen Width",
    //        GUI_METADATA::SLIDER_METADATA{
    //            DEFAULT_PENWIDTH_MIN,
    //            DEFAULT_PENWIDTH_MAX,
    //            GUI_METADATA::SLIDER_METADATA::resolvePenWidth
    //        }
    //    },
    //    &Draw::processChangePenMode
    //},

    //// INPUT
    //{
    //    Command_OLD{COMMAND_ID::INPUT_CLI_ENABLE, Command_OLD::APP::INPUT_MODE::CLI, false},
    //    COMMAND_PROCESSOR_ID::CLI_HANDLER,
    //    SDL_SCANCODE_0,
    //    std::nullopt,
    //    std::nullopt,
    //    std::nullopt
    //},
    //{
    //    Command_OLD{COMMAND_ID::INPUT_MOUSE_ENABLE, Command_OLD::APP::INPUT_MODE::MOUSE, false},
    //    COMMAND_PROCESSOR_ID::MOUSE_HANDLER,
    //    SDL_SCANCODE_3,
    //    std::nullopt,
    //    std::nullopt,
    //    std::nullopt
    //},

    //// RESET
    //{
    //    Command_OLD{COMMAND_ID::RESET_CANVAS, Command_OLD::META::RESET::RESET_CANVAS, false},
    //    COMMAND_PROCESSOR_ID::DRAW_HANDLER,
    //    SDL_SCANCODE_C,
    //    std::nullopt,
    //    "reset_canvas",
    //    GUI_METADATA{
    //        GUI_METADATA::HEADER::FILE,
    //        "Reset Canvas"
    //    },
    //    &Draw::processClearCanvas
    //},
    //{
    //    Command_OLD{COMMAND_ID::RESET_ACTION_QUEUE, Command_OLD::META::RESET::RESET_ACTION_QUEUE, false},
    //    COMMAND_PROCESSOR_ID::ACTION_HANDLER,
    //    std::nullopt,
    //    std::nullopt,
    //    "reset_action_queue",
    //    std::nullopt,
    //    &Action::processClearActionQueue
    //},
    //{
    //    Command_OLD{COMMAND_ID::RESET_ALL, Command_OLD::META::RESET::RESET_ALL, false},
    //    COMMAND_PROCESSOR_ID::CANVAS_HANDLER,
    //    SDL_SCANCODE_R,
    //    std::nullopt,
    //    "reset_all",
    //    GUI_METADATA{
    //        GUI_METADATA::HEADER::FILE,
    //        "Reset All (Includes undo/redo history)"
    //    },
    //    &Canvas::processResetAll
    //},
    //{
    //    Command_OLD{COMMAND_ID::RESET_CURSOR, Command_OLD::META::RESET::RESET_CURSOR, false},
    //    COMMAND_PROCESSOR_ID::CURSOR_HANDLER,
    //    SDL_SCANCODE_9,
    //    std::nullopt,
    //    "reset_cursor",
    //    std::nullopt,
    //    &Cursor::processMoveCursor
    //},

    //// UNDO / REDO
    //{
    //    Command_OLD{COMMAND_ID::UNDO, Command_OLD::APP::UNDO_REDO::UNDO, false},
    //    COMMAND_PROCESSOR_ID::ACTION_HANDLER,
    //    SDL_SCANCODE_N,
    //    std::nullopt,
    //    "undo",
    //    GUI_METADATA{
    //        GUI_METADATA::HEADER::EDIT,
    //        "Undo"
    //    },
    //    &Action::processUndo
    //},
    //{
    //    Command_OLD{COMMAND_ID::REDO, Command_OLD::APP::UNDO_REDO::REDO, false},
    //    COMMAND_PROCESSOR_ID::ACTION_HANDLER,
    //    SDL_SCANCODE_M,
    //    std::nullopt,
    //    "redo",
    //    GUI_METADATA{
    //        GUI_METADATA::HEADER::EDIT,
    //        "Redo"
    //    },
    //    &Action::processRedo
    //},
    //{
    //    Command_OLD{COMMAND_ID::CANVAS_RESIZE_SET_PAYLOAD, Command_OLD::META::CANVAS_CHANGE_SIZE::SET_TO_PAYLOAD, false, coordinate{400, 400}},
    //    COMMAND_PROCESSOR_ID::CANVAS_HANDLER,
    //    SDL_SCANCODE_7,
    //    std::nullopt,
    //    "resize",
    //    std::nullopt,
    //    &Canvas::processCanvasSize
    //},
    //{
    //    Command_OLD{COMMAND_ID::CANVAS_RESIZE_SET_HEIGHT, Command_OLD::META::CANVAS_CHANGE_SIZE::SET_HEIGHT_TO_PAYLOAD, false, 400},
    //    COMMAND_PROCESSOR_ID::CANVAS_HANDLER,
    //    std::nullopt,
    //    std::nullopt,
    //    "resize_height",
    //    GUI_METADATA{
    //        GUI_METADATA::HEADER::FILE,
    //        "Adjust canvas height",
    //        GUI_METADATA::SLIDER_METADATA{
    //            DEFAULT_CANVAS_HEIGHT_MIN,
    //            DEFAULT_CANVAS_HEIGHT_MAX,
    //            GUI_METADATA::SLIDER_METADATA::resolveCanvasHeight
    //        }
    //    },
    //    &Canvas::processCanvasSize
    //},
    //{
    //    Command_OLD{COMMAND_ID::CANVAS_RESIZE_SET_WIDTH, Command_OLD::META::CANVAS_CHANGE_SIZE::SET_WIDTH_TO_PAYLOAD, false, 400},
    //    COMMAND_PROCESSOR_ID::CANVAS_HANDLER,
    //    std::nullopt,
    //    std::nullopt,
    //    "resize_width",
    //    GUI_METADATA{
    //        GUI_METADATA::HEADER::FILE,
    //        "Adjust canvas width",
    //        GUI_METADATA::SLIDER_METADATA{
    //            DEFAULT_CANVAS_WIDTH_MIN,
    //            DEFAULT_CANVAS_WIDTH_MAX,
    //            GUI_METADATA::SLIDER_METADATA::resolveCanvasWidth
    //        }
    //    },
    //    &Canvas::processCanvasSize
    //},

    } };


//// NEW COMMAND REPO

New_Command_Repo NEW_COMMAND_REPO = { {

        // ============================================================
        // MOVEMENT
        // ============================================================

        {
            .ID = COMMAND_ID::MOVE_UP,

            .commandMetadata = {
                .commandDescription = "Move the cursor up.",
                .argumentMetadata = {
                    .arguments = {},
                    .defaultArgs = {}
                }
            },

            .inputMetadata = {
                .keyboard = Command::Keyboard_Metadata{
                    SDL_SCANCODE_W,
                    true
                },
                .cli = Command::CLI_Metadata{
                    "up",
                    "Move the cursor up."
                }
            },

            .processor = &Cursor::processMoveCursor
        },

        {
            .ID = COMMAND_ID::MOVE_DOWN,

            .commandMetadata = {
                .commandDescription = "Move the cursor down.",
                .argumentMetadata = {
                    .arguments = {},
                    .defaultArgs = {}
                }
            },

            .inputMetadata = {
                .keyboard = Command::Keyboard_Metadata{
                    SDL_SCANCODE_S,
                    true
                },
                .cli = Command::CLI_Metadata{
                    "down",
                    "Move the cursor down."
                }
            },

            .processor = &Cursor::processMoveCursor
        },

        {
            .ID = COMMAND_ID::MOVE_LEFT,

            .commandMetadata = {
                .commandDescription = "Move the cursor left.",
                .argumentMetadata = {
                    .arguments = {},
                    .defaultArgs = {}
                }
            },

            .inputMetadata = {
                .keyboard = Command::Keyboard_Metadata{
                    SDL_SCANCODE_A,
                    true
                },
                .cli = Command::CLI_Metadata{
                    "left",
                    "Move the cursor left."
                }
            },

            .processor = &Cursor::processMoveCursor
        },

        {
            .ID = COMMAND_ID::MOVE_RIGHT,

            .commandMetadata = {
                .commandDescription = "Move the cursor right.",
                .argumentMetadata = {
                    .arguments = {},
                    .defaultArgs = {}
                }
            },

            .inputMetadata = {
                .keyboard = Command::Keyboard_Metadata{
                    SDL_SCANCODE_D,
                    true
                },
                .cli = Command::CLI_Metadata{
                    "right",
                    "Move the cursor right."
                }
            },

            .processor = &Cursor::processMoveCursor
        },

        {
            .ID = COMMAND_ID::MOVE_RESET,

            .commandMetadata = {
                .commandDescription = "Reset the cursor to its origin.",
                .argumentMetadata = {
                    .arguments = {},
                    .defaultArgs = {}
                }
            },

            .inputMetadata = {
                .keyboard = Command::Keyboard_Metadata{
                    SDL_SCANCODE_Q,
                    false
                },
                .cli = Command::CLI_Metadata{
                    "reset",
                    "Reset the cursor to its saved origin."
                }
            },

            .processor = &Cursor::processMoveCursor
        },

        {
            .ID = COMMAND_ID::MOVE_SET_POINT,

            .commandMetadata = {
                .commandDescription = "Move the cursor to a specified coordinate.",

                .argumentMetadata = {
                    .arguments = {
                        {
                            Command::ARGTYPE::COORDINATE,
                            "position",
                            "Destination coordinate.",
                            std::nullopt
                        }
                    },

                    .defaultArgs = {
                        DEFAULT_CURSOR_POINT
                    }
                }
            },

            .inputMetadata = {
                .cli = Command::CLI_Metadata{
                    "move",
                    "Move the cursor to a specified coordinate."
                }
            },

            .processor = &Cursor::processMoveCursor
        },

        {
            .ID = COMMAND_ID::MOVE_SAVE_ORIGIN,

            .commandMetadata = {
                .commandDescription = "Save the current cursor position as its origin.",
                .argumentMetadata = {
                    .arguments = {},
                    .defaultArgs = {}
                }
            },

            .inputMetadata = {
                .keyboard = Command::Keyboard_Metadata{
                    SDL_SCANCODE_2,
                    false
                },
                .cli = Command::CLI_Metadata{
                    "save_origin",
                    "Save the current cursor position as the reset origin."
                }
            },

            .processor = &Cursor::processChangeOrigin
        },


    // ============================================================
    // DRAW
    // ============================================================

    {
        .ID = COMMAND_ID::DRAW_CIRCLE,

        .commandMetadata = {
            .commandDescription = "Draw a circle.",
            .argumentMetadata = {
                .arguments = {},
                .defaultArgs = {}
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_G,
                false
            },
            .cli = Command::CLI_Metadata{
                "circle",
                "Draw a circle using the current drawing settings."
            }
        },

        .processor = &Draw::processCircle
    },

    {
        .ID = COMMAND_ID::DRAW_FILL_PAYLOAD,

        .commandMetadata = {
            .commandDescription = "Flood-fill using a specified colour.",

            .argumentMetadata = {
                .arguments = {
                    {
                        Command::ARGTYPE::COLOUR,
                        "colour",
                        "Colour used for the fill.",
                        std::nullopt
                    }
                },

                .defaultArgs = {
                    DEFAULT_DRAW_COLOUR
                }
            }
        },

        .inputMetadata = {
            .cli = Command::CLI_Metadata{
                "fill",
                "Flood-fill the region under the cursor using a specified colour."
            }
        },

        .processor = &Draw::processFill
    },

    {
        .ID = COMMAND_ID::DRAW_FILL_DRAWCOLOUR,

        .commandMetadata = {
            .commandDescription = "Flood-fill using the current draw colour.",
            .argumentMetadata = {
                .arguments = {},
                .defaultArgs = {}
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_F,
                false
            }
        },

        .processor = &Draw::processFill
    },


    // ============================================================
    // COLOUR
    // ============================================================

    {
        .ID = COMMAND_ID::COLOUR_SET_DEFAULT,

        .commandMetadata = {
            .commandDescription = "Reset the active colour to its default.",
            .argumentMetadata = {
                .arguments = {},
                .defaultArgs = {}
            }
        },

        .inputMetadata = {
            .cli = Command::CLI_Metadata{
                "default_colour",
                "Reset the active colour to its default value."
            }
        },

        .processor = &Draw::processChangeColour
    },

    {
        .ID = COMMAND_ID::COLOUR_SET_RANDOM,

        .commandMetadata = {
            .commandDescription = "Set the active colour to a random colour.",
            .argumentMetadata = {
                .arguments = {},
                .defaultArgs = {}
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_J,
                false
            },
            .cli = Command::CLI_Metadata{
                "random_colour",
                "Set the active colour to a randomly generated colour."
            }
        },

        .processor = &Draw::processChangeColour
    },

    {
        .ID = COMMAND_ID::COLOUR_SET_DRAW,

        .commandMetadata = {
            .commandDescription = "Set the draw colour.",

            .argumentMetadata = {
                .arguments = {
                    {
                        Command::ARGTYPE::COLOUR,
                        "colour",
                        "New draw colour.",
                        std::nullopt
                    }
                },

                .defaultArgs = {
                    DEFAULT_DRAW_COLOUR
                }
            }
        },

        .inputMetadata = {
            .cli = Command::CLI_Metadata{
                "colour_draw",
                "Set the current draw colour."
            },

            .gui = Command::GUI_Metadata{
                Command::HEADER::TOOLS,
                "Set draw colour",
                Command::GUI_FUNCTION_TYPE::COLOUR,

                Command::COLOUR_METADATA{
                    &Command::resolveDrawColourChange
                }
            }
        },

        .processor = &Draw::processChangeColour
    },

    {
        .ID = COMMAND_ID::COLOUR_SET_PICK,

        .commandMetadata = {
            .commandDescription = "Pick the colour under the cursor.",
            .argumentMetadata = {
                .arguments = {},
                .defaultArgs = {}
            }
        },

        .inputMetadata = {
            .mouse = Command::Mouse_Metadata{
                SDL_BUTTON_MIDDLE
            }
        },

        .processor = &Draw::processChangeColour
    },

    {
        .ID = COMMAND_ID::COLOUR_SET_BACKGROUND,

        .commandMetadata = {
            .commandDescription = "Set the background colour.",

            .argumentMetadata = {
                .arguments = {
                    {
                        Command::ARGTYPE::COLOUR,
                        "colour",
                        "New background colour.",
                        std::nullopt
                    }
                },

                .defaultArgs = {
                    DEFAULT_DRAW_COLOUR
                }
            }
        },

        .inputMetadata = {
            .cli = Command::CLI_Metadata{
                "colour_background",
                "Set the current background colour."
            },

            .gui = Command::GUI_Metadata{
                Command::HEADER::TOOLS,
                "Set background colour",
                Command::GUI_FUNCTION_TYPE::COLOUR,

                Command::COLOUR_METADATA{
                    Command::resolveBackgroundColourChange
                }
            }
        },

        .processor = &Draw::processChangeColour
    },


    // ============================================================
    // PEN MODE
    // ============================================================

    {
        .ID = COMMAND_ID::PENMODE_DRAW,

        .commandMetadata = {
            .commandDescription = "Switch the pen to normal drawing mode.",
            .argumentMetadata = {
                .arguments = {},
                .defaultArgs = {}
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_Y,
                false
            },

            .cli = Command::CLI_Metadata{
                "penmode_draw",
                "Switch the pen to normal drawing mode."
            },

            .gui = Command::GUI_Metadata{
                Command::HEADER::TOOLS,
                "Draw",
                Command::GUI_FUNCTION_TYPE::BINARY,
                std::monostate{}
            }
        },

        .processor = &Draw::processChangePenMode
    },

    {
        .ID = COMMAND_ID::PENMODE_RUBBER,

        .commandMetadata = {
            .commandDescription = "Switch the pen to rubber mode.",
            .argumentMetadata = {
                .arguments = {},
                .defaultArgs = {}
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_U,
                false
            },

            .cli = Command::CLI_Metadata{
                "penmode_rubber",
                "Switch the pen to rubber mode."
            },

            .gui = Command::GUI_Metadata{
                Command::HEADER::TOOLS,
                "Rubber",
                Command::GUI_FUNCTION_TYPE::BINARY,
                std::monostate{}
            }
        },

        .processor = &Draw::processChangePenMode
    },

    {
        .ID = COMMAND_ID::PENMODE_RAINBOW,

        .commandMetadata = {
            .commandDescription = "Switch the pen to rainbow mode.",
            .argumentMetadata = {
                .arguments = {},
                .defaultArgs = {}
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_E,
                false
            },

            .cli = Command::CLI_Metadata{
                "penmode_rainbow",
                "Switch the pen to rainbow drawing mode."
            },

            .gui = Command::GUI_Metadata{
                Command::HEADER::TOOLS,
                "Rainbow",
                Command::GUI_FUNCTION_TYPE::BINARY,
                std::monostate{}
            }
        },

        .processor = &Draw::processChangePenMode
    },


    // ============================================================
    // PEN
    // ============================================================

    {
        .ID = COMMAND_ID::PEN_DOWN,

        .commandMetadata = {
            .commandDescription = "Perform a discrete pen-down action.",

            .argumentMetadata = {
                .arguments = {
                    {
                        Command::ARGTYPE::INT,
                        "held",
                        "Whether the pen-down input represents a held input.",
                        std::pair<Command::argument, Command::argument>{0, 1}
                    }
                },

                .defaultArgs = {
                    0
                }
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_1,
                false
            },

            .cli = Command::CLI_Metadata{
                "pen",
                "Perform a discrete pen-down action."
            }
        },

        .processor = &Draw::processPenDown
    },

    {
        .ID = COMMAND_ID::PEN_HELD_DOWN,

        .commandMetadata = {
            .commandDescription = "Perform a continuous held pen-down action.",

            .argumentMetadata = {
                .arguments = {
                    {
                        Command::ARGTYPE::INT,
                        "held",
                        "Whether the pen-down input represents a held input.",
                        std::pair<Command::argument, Command::argument>{0, 1}
                    }
                },

                .defaultArgs = {
                    1
                }
            }
        },

        .inputMetadata = {
            .mouse = Command::Mouse_Metadata{
                SDL_BUTTON_LMASK
            }
        },

        .processor = &Draw::processPenDown
    },


    // ============================================================
    // DRAW STEP
    // ============================================================

    {
        .ID = COMMAND_ID::DRAWSTEP_DECREASE,

        .commandMetadata = {
            .commandDescription = "Decrease the cursor draw step.",

            .argumentMetadata = {
                .arguments = {
                    {
                        Command::ARGTYPE::INT,
                        "change",
                        "Amount to add to the current draw step.",
                        std::nullopt
                    }
                },

                .defaultArgs = {
                    -1
                }
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_Z,
                false
            },

            .cli = Command::CLI_Metadata{
                "step_down",
                "Decrease the current draw step."
            }
        },

        .processor = &Cursor::processChangeDrawstep
    },

    {
        .ID = COMMAND_ID::DRAWSTEP_INCREASE,

        .commandMetadata = {
            .commandDescription = "Increase the cursor draw step.",

            .argumentMetadata = {
                .arguments = {
                    {
                        Command::ARGTYPE::INT,
                        "change",
                        "Amount to add to the current draw step.",
                        std::nullopt
                    }
                },

                .defaultArgs = {
                    1
                }
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_X,
                false
            },

            .cli = Command::CLI_Metadata{
                "step_up",
                "Increase the current draw step."
            }
        },

        .processor = &Cursor::processChangeDrawstep
    },

    {
        .ID = COMMAND_ID::DRAWSTEP_SET,

        .commandMetadata = {
            .commandDescription = "Set the cursor draw step.",

            .argumentMetadata = {
                .arguments = {
                    {
                        Command::ARGTYPE::INT,
                        "step",
                        "New cursor draw step.",

                        std::pair<Command::argument, Command::argument>{
                            DEFAULT_DRAWSTEP_MIN,
                            DEFAULT_DRAWSTEP_MAX
                        }
                    }
                },

                .defaultArgs = {
                    DEFAULT_DRAWSTEP_CUR
                }
            }
        },

        .inputMetadata = {
            .cli = Command::CLI_Metadata{
                "step_set",
                "Set the cursor draw step to a specified value."
            },

            .gui = Command::GUI_Metadata{
                Command::HEADER::TOOLS,
                "Change Drawstep",
                Command::GUI_FUNCTION_TYPE::SLIDER,

                Command::SLIDER_METADATA{
                    DEFAULT_DRAWSTEP_MIN,
                    DEFAULT_DRAWSTEP_MAX,
                    Command::resolveDrawstep
                }
            }
        },

        .processor = &Cursor::processChangeDrawstep
    },


    // ============================================================
    // PEN WIDTH
    // ============================================================

    {
        .ID = COMMAND_ID::PEN_WIDTH_DECREASE,

        .commandMetadata = {
            .commandDescription = "Decrease the pen width.",

            .argumentMetadata = {
                .arguments = {
                    {
                        Command::ARGTYPE::INT,
                        "change",
                        "Amount to add to the current pen width.",
                        std::nullopt
                    }
                },

                .defaultArgs = {
                    -1
                }
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_V,
                false
            },

            .cli = Command::CLI_Metadata{
                "pen_thinner",
                "Decrease the current pen width."
            }
        },

        .processor = &Draw::processChangePenWidth
    },

    {
        .ID = COMMAND_ID::PEN_WIDTH_INCREASE,

        .commandMetadata = {
            .commandDescription = "Increase the pen width.",

            .argumentMetadata = {
                .arguments = {
                    {
                        Command::ARGTYPE::INT,
                        "change",
                        "Amount to add to the current pen width.",
                        std::nullopt
                    }
                },

                .defaultArgs = {
                    1
                }
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_B,
                false
            },

            .cli = Command::CLI_Metadata{
                "pen_thicker",
                "Increase the current pen width."
            }
        },

        .processor = &Draw::processChangePenWidth
    },

    {
        .ID = COMMAND_ID::PEN_SET,

        .commandMetadata = {
            .commandDescription = "Set the pen width.",

            .argumentMetadata = {
                .arguments = {
                    {
                        Command::ARGTYPE::INT,
                        "width",
                        "New pen width.",

                        std::pair<Command::argument, Command::argument>{
                            DEFAULT_PENWIDTH_MIN,
                            DEFAULT_PENWIDTH_MAX
                        }
                    }
                },

                .defaultArgs = {
                    DEFAULT_PENWIDTH_CUR
                }
            }
        },

        .inputMetadata = {
            .cli = Command::CLI_Metadata{
                "pen_set",
                "Set the pen width to a specified value."
            },

            .gui = Command::GUI_Metadata{
                Command::HEADER::TOOLS,
                "Change Pen Width",
                Command::GUI_FUNCTION_TYPE::SLIDER,

                Command::SLIDER_METADATA{
                    DEFAULT_PENWIDTH_MIN,
                    DEFAULT_PENWIDTH_MAX,
                    Command::resolvePenWidth
                }
            }
        },

        .processor = &Draw::processChangePenWidth
    },


    // ============================================================
    // INPUT
    // ============================================================

    {
        .ID = COMMAND_ID::INPUT_CLI_ENABLE,

        .commandMetadata = {
            .commandDescription = "Enable CLI input mode.",
            .argumentMetadata = {
                .arguments = {},
                .defaultArgs = {}
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_0,
                false
            }
        },

        .processor = nullptr
    },

    {
        .ID = COMMAND_ID::INPUT_MOUSE_ENABLE,

        .commandMetadata = {
            .commandDescription = "Enable mouse input mode.",
            .argumentMetadata = {
                .arguments = {},
                .defaultArgs = {}
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_3,
                false
            }
        },

        .processor = nullptr
    },


    // ============================================================
    // RESET
    // ============================================================

    {
        .ID = COMMAND_ID::RESET_CANVAS,

        .commandMetadata = {
            .commandDescription = "Reset the canvas.",
            .argumentMetadata = {
                .arguments = {},
                .defaultArgs = {}
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_C,
                false
            },

            .cli = Command::CLI_Metadata{
                "reset_canvas",
                "Reset the canvas contents."
            },

            .gui = Command::GUI_Metadata{
                Command::HEADER::FILE,
                "Reset Canvas",
                Command::GUI_FUNCTION_TYPE::BINARY,
                std::monostate{}
            }
        },

        .processor = &Draw::processClearCanvas
    },

    {
        .ID = COMMAND_ID::RESET_ACTION_QUEUE,

        .commandMetadata = {
            .commandDescription = "Clear the undo and redo history.",
            .argumentMetadata = {
                .arguments = {},
                .defaultArgs = {}
            }
        },

        .inputMetadata = {
            .cli = Command::CLI_Metadata{
                "reset_action_queue",
                "Clear all stored undo and redo history."
            }
        },

        .processor = &Action::processClearActionQueue
    },

    {
        .ID = COMMAND_ID::RESET_ALL,

        .commandMetadata = {
            .commandDescription = "Reset the complete application state.",
            .argumentMetadata = {
                .arguments = {},
                .defaultArgs = {}
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_R,
                false
            },

            .cli = Command::CLI_Metadata{
                "reset_all",
                "Reset all state, including undo and redo history."
            },

            .gui = Command::GUI_Metadata{
                Command::HEADER::FILE,
                "Reset All (Includes undo/redo history)",
                Command::GUI_FUNCTION_TYPE::BINARY,
                std::monostate{}
            }
        },

        .processor = &Canvas::processResetAll
    },

    {
        .ID = COMMAND_ID::RESET_CURSOR,

        .commandMetadata = {
            .commandDescription = "Reset the cursor.",
            .argumentMetadata = {
                .arguments = {},
                .defaultArgs = {}
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_9,
                false
            },

            .cli = Command::CLI_Metadata{
                "reset_cursor",
                "Reset the cursor position."
            }
        },

        .processor = &Cursor::processMoveCursor
    },


    // ============================================================
    // UNDO / REDO
    // ============================================================

    {
        .ID = COMMAND_ID::UNDO,

        .commandMetadata = {
            .commandDescription = "Undo the most recent action.",
            .argumentMetadata = {
                .arguments = {},
                .defaultArgs = {}
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_N,
                false
            },

            .cli = Command::CLI_Metadata{
                "undo",
                "Undo the most recent action."
            },

            .gui = Command::GUI_Metadata{
                Command::HEADER::EDIT,
                "Undo",
                Command::GUI_FUNCTION_TYPE::BINARY,
                std::monostate{}
            }
        },

        .processor = &Action::processUndo
    },

    {
        .ID = COMMAND_ID::REDO,

        .commandMetadata = {
            .commandDescription = "Redo the most recently undone action.",
            .argumentMetadata = {
                .arguments = {},
                .defaultArgs = {}
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_M,
                false
            },

            .cli = Command::CLI_Metadata{
                "redo",
                "Redo the most recently undone action."
            },

            .gui = Command::GUI_Metadata{
                Command::HEADER::EDIT,
                "Redo",
                Command::GUI_FUNCTION_TYPE::BINARY,
                std::monostate{}
            }
        },

        .processor = &Action::processRedo
    },


    // ============================================================
    // CANVAS SIZE
    // ============================================================

    {
        .ID = COMMAND_ID::CANVAS_RESIZE_SET_PAYLOAD,

        .commandMetadata = {
            .commandDescription = "Resize the canvas.",

            .argumentMetadata = {
                .arguments = {
                    {
                        Command::ARGTYPE::COORDINATE,
                        "size",
                        "New canvas width and height.",
                        std::nullopt
                    }
                },

                .defaultArgs = {
                    coordinate{400, 400}
                }
            }
        },

        .inputMetadata = {
            .keyboard = Command::Keyboard_Metadata{
                SDL_SCANCODE_7,
                false
            },

            .cli = Command::CLI_Metadata{
                "resize",
                "Resize the canvas to a specified width and height."
            }
        },

        .processor = &Canvas::processCanvasSize
    },

    {
        .ID = COMMAND_ID::CANVAS_RESIZE_SET_HEIGHT,

        .commandMetadata = {
            .commandDescription = "Set the canvas height.",

            .argumentMetadata = {
                .arguments = {
                    {
                        Command::ARGTYPE::INT,
                        "height",
                        "New canvas height in pixels.",

                        std::pair<Command::argument, Command::argument>{
                            DEFAULT_CANVAS_HEIGHT_MIN,
                            DEFAULT_CANVAS_HEIGHT_MAX
                        }
                    }
                },

                .defaultArgs = {
                    DEFAULT_CANVAS_HEIGHT_CUR
                }
            }
        },

        .inputMetadata = {
            .cli = Command::CLI_Metadata{
                "resize_height",
                "Set the canvas height."
            },

            .gui = Command::GUI_Metadata{
                Command::HEADER::FILE,
                "Adjust canvas height",
                Command::GUI_FUNCTION_TYPE::SLIDER,

                Command::SLIDER_METADATA{
                    DEFAULT_CANVAS_HEIGHT_MIN,
                    DEFAULT_CANVAS_HEIGHT_MAX,
                    Command::resolveCanvasHeight
                }
            }
        },

        .processor = &Canvas::processCanvasSize
    },

    {
        .ID = COMMAND_ID::CANVAS_RESIZE_SET_WIDTH,

        .commandMetadata = {
            .commandDescription = "Set the canvas width.",

            .argumentMetadata = {
                .arguments = {
                    {
                        Command::ARGTYPE::INT,
                        "width",
                        "New canvas width in pixels.",

                        std::pair<Command::argument, Command::argument>{
                            DEFAULT_CANVAS_WIDTH_MIN,
                            DEFAULT_CANVAS_WIDTH_MAX
                        }
                    }
                },

                .defaultArgs = {
                    DEFAULT_CANVAS_WIDTH_CUR
                }
            }
        },

        .inputMetadata = {
            .cli = Command::CLI_Metadata{
                "resize_width",
                "Set the canvas width."
            },

            .gui = Command::GUI_Metadata{
                Command::HEADER::FILE,
                "Adjust canvas width",
                Command::GUI_FUNCTION_TYPE::SLIDER,

                Command::SLIDER_METADATA{
                    DEFAULT_CANVAS_WIDTH_MIN,
                    DEFAULT_CANVAS_WIDTH_MAX,
                    Command::resolveCanvasWidth
                }
            }
        },

        .processor = &Canvas::processCanvasSize
    }

} };