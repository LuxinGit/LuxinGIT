#include "COMMAND REPO.h"
#include "Application/Application.h"
#include "Command/Command.h"
//// NEW COMMAND REPO

namespace Definition = Command::Definition;
namespace inp = Definition::Input;
namespace GUI = inp::GUI;

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
                .keyboard = inp::Keyboard_Metadata{
                    SDL_SCANCODE_W,
                    true
                },
                .cli = inp::CLI_Metadata{
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
                .keyboard = inp::Keyboard_Metadata{
                    SDL_SCANCODE_S,
                    true
                },
                .cli = inp::CLI_Metadata{
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
                .keyboard = inp::Keyboard_Metadata{
                    SDL_SCANCODE_A,
                    true
                },
                .cli = inp::CLI_Metadata{
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
                .keyboard = inp::Keyboard_Metadata{
                    SDL_SCANCODE_D,
                    true
                },
                .cli = inp::CLI_Metadata{
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
                .keyboard = inp::Keyboard_Metadata{
                    SDL_SCANCODE_Q,
                    false
                },
                .cli = inp::CLI_Metadata{
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
                            Command::Argument::ARGTYPE::COORDINATE,
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
                .cli = inp::CLI_Metadata{
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
                .keyboard = inp::Keyboard_Metadata{
                    SDL_SCANCODE_2,
                    false
                },
                .cli = inp::CLI_Metadata{
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
                .arguments = {
                    {
                        Command::Argument::ARGTYPE::INT,
                        "radius",
                        "Radius of the circle",
                        std::nullopt,
                        false
                    },
                    {
                        Command::Argument::ARGTYPE::COORDINATE,
                        "centre coordinate",
                        "Coordinate used for the circle's centre.",
                        std::nullopt,
                        false
                    },
                    {
                        Command::Argument::ARGTYPE::COLOUR,
                        "colour",
                        "Colour used to draw the circle",
                        std::nullopt,
                        false
                    },
                    {
                        Command::Argument::ARGTYPE::INT,
                        "fill [bool]",
                        "Whether or not we're filling the circle.  Int -> bool (so any non-zero value = true)",
                        std::nullopt,
                        false
                    }
                },
                .defaultArgs = {
                    std::monostate(),
                    std::monostate(),
                    std::monostate(),
                    std::monostate()
                }   
            }
        },

        .inputMetadata = {
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_G,
                false
            },
            .cli = inp::CLI_Metadata{
                "circle",
                R"(Draw a circle.
Arguments are:
    [0] {Optional} Radius. Uses 10* the drawstep as default.
    [1] {Optional} Centre coordinate. Uses the cursor as default.
    [2] {Optional} Colour. Uses the active colour as default.
    [3] {Optional} Fill? Any non-zero input == true.)"
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
                        Command::Argument::ARGTYPE::COLOUR,
                        "colour",
                        "Colour used for the fill.",
                        std::nullopt,
                        false
                    },
                    {
                        Command::Argument::ARGTYPE::COORDINATE,
                        "coordinate",
                        "Colour used for the fill location.",
                        std::pair<coordinate, coordinate>{
                            { DEFAULT_CANVAS_WIDTH_MIN, DEFAULT_CANVAS_HEIGHT_MIN },
                            { DEFAULT_CANVAS_WIDTH_MAX, DEFAULT_CANVAS_HEIGHT_MAX }
                        },
                        false
                    }
                },

                .defaultArgs = {
                    std::monostate(),
                    std::monostate()
                }
            }
        },

        .inputMetadata = {
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_F,
                false
            },
            .cli = inp::CLI_Metadata{
                "fill",
                R"(Flood-fill a region.
Argumments are:
    [0] {Optional} Colour. Uses the active colour by default.
    [1] {Optional} Coordinate. Uses the cursor by default.)"
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
            .cli = inp::CLI_Metadata{
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_J,
                false
            },
            .cli = inp::CLI_Metadata{
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
                        Command::Argument::ARGTYPE::COLOUR,
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
            .cli = inp::CLI_Metadata{
                "colour_draw",
                "Set the current draw colour."
            },

            .gui = GUI::GUI_Metadata{
                GUI::HEADER::TOOLS,
                "Set draw colour",
                GUI::FUNCTION_TYPE::COLOUR,

               GUI::COLOUR_METADATA{
                    &GUI::Resolver::resolveDrawColourChange
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
            .mouse = inp::Mouse_Metadata{
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
                        Command::Argument::ARGTYPE::COLOUR,
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
            .cli = inp::CLI_Metadata{
                "colour_background",
                "Set the current background colour."
            },

            .gui = GUI::GUI_Metadata{
                GUI::HEADER::TOOLS,
                "Set background colour",
                GUI::FUNCTION_TYPE::COLOUR,

                GUI::COLOUR_METADATA{
                    GUI::Resolver::resolveBackgroundColourChange
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_Y,
                false
            },

            .cli = inp::CLI_Metadata{
                "penmode_draw",
                "Switch the pen to normal drawing mode."
            },

            .gui = GUI::GUI_Metadata{
                GUI::HEADER::TOOLS,
                "Draw",
                GUI::FUNCTION_TYPE::BINARY,
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_U,
                false
            },

            .cli = inp::CLI_Metadata{
                "penmode_rubber",
                "Switch the pen to rubber mode."
            },

            .gui = GUI::GUI_Metadata{
                GUI::HEADER::TOOLS,
                "Rubber",
                GUI::FUNCTION_TYPE::BINARY,
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_E,
                false
            },

            .cli = inp::CLI_Metadata{
                "penmode_rainbow",
                "Switch the pen to rainbow drawing mode."
            },

            .gui = GUI::GUI_Metadata{
                GUI::HEADER::TOOLS,
                "Rainbow",
                GUI::FUNCTION_TYPE::BINARY,
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
                        Command::Argument::ARGTYPE::INT,
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_1,
                false
            },

            .cli = inp::CLI_Metadata{
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
                        Command::Argument::ARGTYPE::INT,
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
            .mouse = inp::Mouse_Metadata{
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
                        Command::Argument::ARGTYPE::INT,
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_Z,
                false
            },

            .cli = inp::CLI_Metadata{
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
                        Command::Argument::ARGTYPE::INT,
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_X,
                false
            },

            .cli = inp::CLI_Metadata{
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
                        Command::Argument::ARGTYPE::INT,
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
            .cli = inp::CLI_Metadata{
                "step_set",
                "Set the cursor draw step to a specified value."
            },

            .gui = GUI::GUI_Metadata{
                GUI::HEADER::TOOLS,
                "Change Drawstep",
                GUI::FUNCTION_TYPE::SLIDER,

                GUI::SLIDER_METADATA{
                    DEFAULT_DRAWSTEP_MIN,
                    DEFAULT_DRAWSTEP_MAX,
                    GUI::Resolver::resolveDrawstep
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
                        Command::Argument::ARGTYPE::INT,
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_V,
                false
            },

            .cli = inp::CLI_Metadata{
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
                        Command::Argument::ARGTYPE::INT,
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_B,
                false
            },

            .cli = inp::CLI_Metadata{
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
                        Command::Argument::ARGTYPE::INT,
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
            .cli = inp::CLI_Metadata{
                "pen_set",
                "Set the pen width to a specified value."
            },

            .gui = GUI::GUI_Metadata{
                GUI::HEADER::TOOLS,
                "Change Pen Width",
                GUI::FUNCTION_TYPE::SLIDER,

                GUI::SLIDER_METADATA{
                    DEFAULT_PENWIDTH_MIN,
                    DEFAULT_PENWIDTH_MAX,
                    GUI::Resolver::resolvePenWidth
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_0,
                false
            }
        },

        .processor = &Input::CLI::openCLI
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_3,
                false
            }
        },

        .processor = &Input::enableMouse
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_C,
                false
            },

            .cli = inp::CLI_Metadata{
                "reset_canvas",
                "Reset the canvas contents."
            },

            .gui = GUI::GUI_Metadata{
                GUI::HEADER::FILE,
                "Reset Canvas",
                GUI::FUNCTION_TYPE::BINARY,
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
            .cli = inp::CLI_Metadata{
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_R,
                false
            },

            .cli = inp::CLI_Metadata{
                "reset_all",
                "Reset all state, including undo and redo history."
            },

            .gui = GUI::GUI_Metadata{
                GUI::HEADER::FILE,
                "Reset All (Includes undo/redo history)",
                GUI::FUNCTION_TYPE::BINARY,
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_9,
                false
            },

            .cli = inp::CLI_Metadata{
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_N,
                false
            },

            .cli = inp::CLI_Metadata{
                "undo",
                "Undo the most recent action."
            },

            .gui = GUI::GUI_Metadata{
                GUI::HEADER::EDIT,
                "Undo",
                GUI::FUNCTION_TYPE::BINARY,
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_M,
                false
            },

            .cli = inp::CLI_Metadata{
                "redo",
                "Redo the most recently undone action."
            },

            .gui = GUI::GUI_Metadata{
                GUI::HEADER::EDIT,
                "Redo",
                GUI::FUNCTION_TYPE::BINARY,
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
                        Command::Argument::ARGTYPE::COORDINATE,
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
            .keyboard = inp::Keyboard_Metadata{
                SDL_SCANCODE_7,
                false
            },

            .cli = inp::CLI_Metadata{
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
                        Command::Argument::ARGTYPE::INT,
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
            .cli = inp::CLI_Metadata{
                "resize_height",
                "Set the canvas height."
            },

            .gui = GUI::GUI_Metadata{
                GUI::HEADER::FILE,
                "Adjust canvas height",
                GUI::FUNCTION_TYPE::SLIDER,

                GUI::SLIDER_METADATA{
                    DEFAULT_CANVAS_HEIGHT_MIN,
                    DEFAULT_CANVAS_HEIGHT_MAX,
                    GUI::Resolver::resolveCanvasHeight
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
                        Command::Argument::ARGTYPE::INT,
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
            .cli = inp::CLI_Metadata{
                "resize_width",
                "Set the canvas width."
            },

            .gui = GUI::GUI_Metadata{
                GUI::HEADER::FILE,
                "Adjust canvas width",
                GUI::FUNCTION_TYPE::SLIDER,

                GUI::SLIDER_METADATA{
                    DEFAULT_CANVAS_WIDTH_MIN,
                    DEFAULT_CANVAS_WIDTH_MAX,
                    GUI::Resolver::resolveCanvasWidth
                }
            }
        },

        .processor = &Canvas::processCanvasSize
    }

} };