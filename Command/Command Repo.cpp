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
            .ID = COMMAND_ID::CURSOR_MOVE,

            .commandMetadata = {
                .commandDescription = "Move the cursor",
                .arguments = {
                    {
                        Command::Argument::ARGTYPE::INT,
                        "setting (DIR/SET/ORIGIN)",
                        "Determines usage of coordinate. If 0, command interpreted as directional command.",
                        std::nullopt
                    },
                    {
                        Command::Argument::ARGTYPE::COORDINATE,
                        "position",
                        "Destination coordinate, OR drawstep usage if setting == 0",
                        std::nullopt
                    }
                },
                .presets = {
                    {"up",          { 0, coordinate{0, -1}}},
                    {"right",       { 0, coordinate{1,  0}}},
                    {"down",        { 0, coordinate{0,  1}}},
                    {"left",        { 0, coordinate{-1, 0}}},
                    {"set_origin",  { 2, coordinate{-1, -1}}} //slightly hacky
                }
            },

            .inputMetadata = {
                .keyboard = {               
                    {
                        inp::Keyboard_Metadata{ SDL_SCANCODE_W, 0, true  },
                        inp::Keyboard_Metadata{ SDL_SCANCODE_D, 1, true  },
                        inp::Keyboard_Metadata{ SDL_SCANCODE_S, 2, true  },
                        inp::Keyboard_Metadata{ SDL_SCANCODE_A, 3, true  },
                        inp::Keyboard_Metadata{ SDL_SCANCODE_9, 4, false }
                    }
                },
                .cli = inp::CLI_Metadata{
                    "Move",
                    "Move the cursor."
                }
            },

            .processor = &Cursor::processMoveCursor
        },
        {
            .ID = COMMAND_ID::CURSOR_ORIGIN,

            .commandMetadata = {
                .commandDescription = "Save the current cursor position as its origin.",
                .arguments = {},
                .presets = {
                    {"save_position", {}},
                }
            },

            .inputMetadata = {
                .keyboard = {
                    {
                        inp::Keyboard_Metadata{ SDL_SCANCODE_2, 0, false  }
                    }
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
            .presets = {
                { "defaultCircle" , {} }
            }
        },

        .inputMetadata = {
            .keyboard = {
                {
                    inp::Keyboard_Metadata{SDL_SCANCODE_G, 0, false}
                } 
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
        .ID = COMMAND_ID::DRAW_FILL,

        .commandMetadata = {
            .commandDescription = "Flood-fill using a specified colour.",
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
            .presets = {
                { "fill at cursor", {}}
            }
        },

        .inputMetadata = {
            .keyboard = {
                {
                    inp::Keyboard_Metadata{SDL_SCANCODE_F, 0, false}
                }
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
        .ID = COMMAND_ID::COLOUR,

            .commandMetadata = {
                .commandDescription = "Change a colour.",
                .arguments = {
                     {
                        Command::Argument::ARGTYPE::INT,
                        "target colour",
                        "Which colour is changed. [0]activeColour, [1]drawColour, [2]backgroundColour",
                        std::pair<int,int>{ 0 , 2 }
                     },
                     {
                        Command::Argument::ARGTYPE::INT,
                        "setting",
                        "Control path for colour retrieval. [0]new colour, [1]getRandomColour, [2]cursorColour, [3]default",
                        std::pair<int,int>{ 0 , 3 }
                     },
                     {
                        Command::Argument::ARGTYPE::COLOUR,
                        "new colour",
                        "The colour that we're changing to.",
                        std::nullopt,
                        false
                     }

                },
                .presets = {
                    { "Random Colour", { 0, 1 }},
                    { "Pick cursor Colour", { 0, 2 }}
                }
        },

            .inputMetadata = {
                .keyboard = {
                    {
                        inp::Keyboard_Metadata{ SDL_SCANCODE_J, 0 }
                    }
                },
                .mouse = {
                    {
                        inp::Mouse_Metadata{ SDL_BUTTON_MIDDLE, 1 }
                    }
                },
                .cli = inp::CLI_Metadata{
        "colour",
        R"(Change a colour.
Arguments are:
    [0] Target colour. [0] activeColour, [1] drawColour, [2] backgroundColour.
    [1] Setting controlling colour retrieval. [0] use provided colour, [1] random colour, [2] cursor colour, [3] default colour.
    [2] {Optional} New colour. Used only when setting == 0.)"
            },
            .gui = GUI::GUI_Metadata{
                GUI::HEADER::TOOLS,
                "Set draw colour",
                GUI::FUNCTION_TYPE::COLOUR,

               GUI::COLOUR_METADATA{
                    &GUI::Resolver::resolveDrawColourChange
               }
            }/*,

            .gui = GUI::GUI_Metadata{
                GUI::HEADER::TOOLS,
                "Set background colour",
                GUI::FUNCTION_TYPE::COLOUR,

                GUI::COLOUR_METADATA{
                    GUI::Resolver::resolveBackgroundColourChange
                }
            }
            */
        },

            .processor = &Draw::processChangeColour
      },

        // ============================================================
        // PEN MODE
        // ============================================================
     {
        .ID = COMMAND_ID::PENMODE,

        .commandMetadata = {
            .commandDescription = "Switch the pen mode.",
            .arguments = {
                {
                    Command::Argument::ARGTYPE::INT,
                    "pen mode",
                    "Which pen mode to set to. [0]Draw, [1]Rubber, [2]Rainbow",
                    std::pair<int,int>{ 0 , 2 }
                },
                {
                    Command::Argument::ARGTYPE::INT,
                    "setting",
                    "Optional parameter for selected pen mode",
                    std::nullopt,
                    false
                },
            },
            .presets = {
                { "Draw",    { 0 } },
                { "Rubber",  { 1 } },
                { "Rainbow", { 2 } }
            }
            
        },

        .inputMetadata = {
            .keyboard = {
                {
                    inp::Keyboard_Metadata{ SDL_SCANCODE_Y, 0},
                    inp::Keyboard_Metadata{ SDL_SCANCODE_U, 1},
                    inp::Keyboard_Metadata{ SDL_SCANCODE_E, 2}
                }
            },

            .cli = inp::CLI_Metadata
                { 
                "penmode",
    R"(Switch the pen mode.
Arguments are:
    [0] Pen mode. [0] Draw, [1] Rubber, [2] Rainbow.
    [1] {Optional} Setting. Additional parameter interpreted by the selected pen mode.)"
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
    // ============================================================
    // PEN
    // ============================================================

    {
        .ID = COMMAND_ID::PEN_DOWN,

        .commandMetadata = {
            .commandDescription = "Enables cursor-based drawing",

            .arguments = {
                {   
                    Command::Argument::ARGTYPE::INT,
                    "type",
                    "Whether the command needs discrete[0] or continuous[1] input",
                    std::pair<int, int >{0, 1},
                }
            },
            .presets = {
                { "Discrete",   { 0 }},
                { "Continuous", { 1 }}
            }
        },

        .inputMetadata = {
            .keyboard = {
                {
                    {   SDL_SCANCODE_1, 0   }
                }
            },
            .mouse = {
                {
                    {   SDL_BUTTON_LMASK, 1   }
                }
            },

            .cli = inp::CLI_Metadata{
                "pen",
                R"(Begin drawing as the cursor moves.
Arguments are:
    [0] Input mode. [0] Discrete, [1] Continuous.
Continuous input is not supported through the CLI.)"
            }
        },

        .processor = &Draw::processPenDown
    },

    // ============================================================
    // DRAW STEP
    // ============================================================

    {
        .ID = COMMAND_ID::CURSOR_DRAWSTEP,

        .commandMetadata = {
            .commandDescription = "Change the difference travelled by the cursor (per input)",
            .arguments = {
                {
                    Command::Argument::ARGTYPE::INT,
                    "setting (ADD/SET)",
                    "Setting to define interpretation of ARG_2",
                    std::pair<int,int>{ 0,1 }
                },
                {
                    Command::Argument::ARGTYPE::INT,
                    "Delta",
                    "Amount to be added to, or have drawstep set to",
                    std::pair<int, int>{ DEFAULT_DRAWSTEP_MIN,DEFAULT_DRAWSTEP_MAX }
                }
            },
            .presets = {
                {   "Increment",    { 0, 1 }  },
                {   "Decrement",    { 0,-1 }  },
                {   "Reset",        { 1, DEFAULT_DRAWSTEP_CUR}  }
            }
        },

        .inputMetadata = {
            .keyboard = {
                {
                    inp::Keyboard_Metadata{ SDL_SCANCODE_X, 0 },
                    inp::Keyboard_Metadata{ SDL_SCANCODE_Z, 1 }
                }
            },

            .cli = inp::CLI_Metadata{
    "drawstep",
    R"(Change the distance travelled by the cursor per input.
Arguments are:
    [0] Setting. [0] Add to the current draw step, [1] Set the draw step directly.
    [1] Delta. Amount to add, or value to set the draw step to.)"
}
        },

        .processor = &Cursor::processChangeDrawstep
    },

    // ============================================================
    // PEN WIDTH
    // ============================================================

{
    .ID = COMMAND_ID::PEN_WIDTH,

    .commandMetadata = {
        .commandDescription = "Change the pen width.",

        .arguments = {
            {
                Command::Argument::ARGTYPE::INT,
                "setting (ADD/SET)",
                "Setting to define interpretation of ARG_2.",
                std::pair<int, int>{ 0, 1 }
            },
            {
                Command::Argument::ARGTYPE::INT,
                "Delta",
                "Amount to add to, or value to set the pen width to.",
                std::pair<int, int>{ DEFAULT_PENWIDTH_MIN, DEFAULT_PENWIDTH_MAX }
            }
        },

        .presets = {
            { "Increment", { 0,  1 } },
            { "Decrement", { 0, -1 } },
            { "Reset",     { 1, DEFAULT_PENWIDTH_CUR } }
        }
    },

    .inputMetadata = {
        .keyboard = {
            {
                inp::Keyboard_Metadata{ SDL_SCANCODE_B, 0 },
                inp::Keyboard_Metadata{ SDL_SCANCODE_V, 1 }
            }
        },

        .cli = inp::CLI_Metadata{
            "penwidth",
            R"(Change the pen width.
Arguments are:
    [0] Setting. [0] Add to the current pen width, [1] Set the pen width directly.
    [1] Delta. Amount to add, or value to set the pen width to.)"
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
            .arguments = {},
            .presets = {
                { "Enable CLI", {} }
            }
        },

        .inputMetadata = {
            .keyboard = {
                {
                    inp::Keyboard_Metadata{ SDL_SCANCODE_0, 0}
                }
            }
        },

        .processor = &Input::CLI::openCLI
    },

    {
        .ID = COMMAND_ID::INPUT_MOUSE_ENABLE,

        .commandMetadata = {
            .commandDescription = "Enable mouse input mode.",
            .arguments = {},
            .presets = {
                { "Enable Mouse", {} }
            }
        },

        .inputMetadata = {
            .keyboard = {
                {
                    inp::Keyboard_Metadata{ SDL_SCANCODE_3, 0 }
                }
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
             = {
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
             = {
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
             = {
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

    // ============================================================
    // UNDO / REDO
    // ============================================================

    {
        .ID = COMMAND_ID::UNDO,

        .commandMetadata = {
            .commandDescription = "Undo the most recent action.",
             = {
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
             = {
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

             = {
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

             = {
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

             = {
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