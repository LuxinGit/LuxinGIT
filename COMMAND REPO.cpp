#include "COMMAND REPO.h"
#include "Action.h"
#include "Canvas.h"

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
                    &Command::GUI_Resolver::resolveDrawColourChange
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
                    Command::GUI_Resolver::resolveBackgroundColourChange
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
                    Command::GUI_Resolver::resolveDrawstep
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
                    Command::GUI_Resolver::resolvePenWidth
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
                        std::pair<coordinate, coordinate>{
                            { DEFAULT_CANVAS_WIDTH_MIN, DEFAULT_CANVAS_HEIGHT_MIN },
                            { DEFAULT_CANVAS_WIDTH_MAX, DEFAULT_CANVAS_HEIGHT_MAX }
                        }
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
                    Command::GUI_Resolver::resolveCanvasHeight
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
                    Command::GUI_Resolver::resolveCanvasWidth
                }
            }
        },

        .processor = &Canvas::processCanvasSize
    }

} };