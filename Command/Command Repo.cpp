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
                std::pair<int, int>{ 0, 2 }
            },
            {
                Command::Argument::ARGTYPE::INT,
                "setting",
                "Control path for colour retrieval. [0]new colour, [1]getRandomColour, [2]cursorColour, [3]default",
                std::pair<int, int>{ 0, 3 }
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
            { "Random Colour",          { 0, 1 } },
            { "Pick cursor Colour",     { 0, 2 } },
            { "Set Draw Colour",        { 1, 0 } },
            { "Set Background Colour",  { 2, 0 } }
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

        .gui = {
            {
                GUI::GUI_Metadata{
                    GUI::HEADER::TOOLS,
                    "Set draw colour",
                    GUI::FUNCTION_TYPE::COLOUR,
                    2,

                    GUI::COLOUR_METADATA{
                        &GUI::Resolver::resolveDrawColourChange
                    }
                },

                GUI::GUI_Metadata{
                    GUI::HEADER::TOOLS,
                    "Set background colour",
                    GUI::FUNCTION_TYPE::COLOUR,
                    3,

                    GUI::COLOUR_METADATA{
                        &GUI::Resolver::resolveBackgroundColourChange
                    }
                }
            }
        }
    },

    .processor = &Draw::processChangeColour
},
    // ============================================================
    // PEN MODE
    // ============================================================
    {
    .ID = COMMAND_ID::PEN_MODE,

    .commandMetadata = {
        .commandDescription = "Switch the pen mode.",

        .arguments = {
            {
                Command::Argument::ARGTYPE::INT,
                "pen mode",
                "Which pen mode to set to. [0]Draw, [1]Rubber, [2]Rainbow",
                std::pair<int, int>{ 0, 2 }
            },
            {
                Command::Argument::ARGTYPE::INT,
                "setting",
                "Optional parameter for selected pen mode",
                std::nullopt,
                false
            }
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
                inp::Keyboard_Metadata{ SDL_SCANCODE_Y, 0 },
                inp::Keyboard_Metadata{ SDL_SCANCODE_U, 1 },
                inp::Keyboard_Metadata{ SDL_SCANCODE_E, 2 }
            }
        },

        .cli = inp::CLI_Metadata{
            "penmode",
            R"(Switch the pen mode.
Arguments are:
    [0] Pen mode. [0] Draw, [1] Rubber, [2] Rainbow.
    [1] {Optional} Setting. Additional parameter interpreted by the selected pen mode.)"
        },

        .gui = {
            {
                GUI::GUI_Metadata{
                    GUI::HEADER::TOOLS,
                    "Draw",
                    GUI::FUNCTION_TYPE::BINARY,
                    0,
                    std::monostate{}
                },

                GUI::GUI_Metadata{
                    GUI::HEADER::TOOLS,
                    "Rubber",
                    GUI::FUNCTION_TYPE::BINARY,
                    1,
                    std::monostate{}
                },

                GUI::GUI_Metadata{
                    GUI::HEADER::TOOLS,
                    "Rainbow",
                    GUI::FUNCTION_TYPE::BINARY,
                    2,
                    std::monostate{}
                }
            }
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
                std::pair<int, int>{ 0, 1 }
            },
            {
                Command::Argument::ARGTYPE::INT,
                "Delta",
                "Amount to be added to, or have drawstep set to",
                std::pair<int, int>{ DEFAULT_DRAWSTEP_MIN, DEFAULT_DRAWSTEP_MAX }
            }
        },

        .presets = {
            { "Decrement", { 0,  1 } },
            { "Increment", { 0,  2 } },            
            { "Reset",     { 1, DEFAULT_DRAWSTEP_CUR } }
        }
    },

    .inputMetadata = {
        .keyboard = {
            {
                inp::Keyboard_Metadata{ SDL_SCANCODE_Z, 0 },
                inp::Keyboard_Metadata{ SDL_SCANCODE_X, 1 } // extremely hacky.
            }
        },

        .cli = inp::CLI_Metadata{
            "drawstep",
            R"(Change the distance travelled by the cursor per input.
Arguments are:
    [0] Setting. [0] Add to the current draw step, [1] Set the draw step directly.
    [1] Delta. Amount to add, or value to set the draw step to.)"
        },

        .gui = {
    {
        GUI::GUI_Metadata{
            GUI::HEADER::TOOLS,
            "Draw step",
            GUI::FUNCTION_TYPE::SLIDER,
            2,
            GUI::SLIDER_METADATA{
                DEFAULT_DRAWSTEP_MIN,
                DEFAULT_DRAWSTEP_MAX,
                1,
                GUI::Resolver::resolveDrawstep
            }
        }
    }
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
            { "Decrement", { 0,  0 } },
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
        },

        .gui = {
    {
        GUI::GUI_Metadata{
            GUI::HEADER::TOOLS,
            "Pen width",
            GUI::FUNCTION_TYPE::SLIDER,
            2,
            GUI::SLIDER_METADATA{
                DEFAULT_PENWIDTH_MIN,
                DEFAULT_PENWIDTH_MAX,
                1,
                GUI::Resolver::resolvePenWidth
            }
        }
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
        .arguments = {},
        .presets = {
            { "Enable CLI", {} }
        }
    },

    .inputMetadata = {
        .keyboard = {
            {
                inp::Keyboard_Metadata{ SDL_SCANCODE_0, 0 }
            }
        },

        .gui = {
            {
                GUI::GUI_Metadata{
                    GUI::HEADER::FILE,
                    "Enable CLI",
                    GUI::FUNCTION_TYPE::BINARY,
                    0,
                    std::monostate{}
                }
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
        },

        .gui = {
            {
                GUI::GUI_Metadata{
                    GUI::HEADER::FILE,
                    "Enable Mouse",
                    GUI::FUNCTION_TYPE::BINARY,
                    0,
                    std::monostate{}
                }
            }
        }
    },

    .processor = &Input::enableMouse
},


    // ============================================================
    // RESET
    // ============================================================

{
    .ID = COMMAND_ID::RESET,

    .commandMetadata = {
        .commandDescription = "Reset application state.",

        .arguments = {
            {
                Command::Argument::ARGTYPE::INT,
                "action queue",
                "Whether to reset the undo/redo action queue. [0] No, [1] Yes.",
                std::pair<int, int>{ 0, 1 }
            },
            {
                Command::Argument::ARGTYPE::INT,
                "canvas",
                "Whether to reset the canvas contents. [0] No, [1] Yes.",
                std::pair<int, int>{ 0, 1 }
            },
            {
                Command::Argument::ARGTYPE::INT,
                "cursor",
                "Whether to reset the cursor position. [0] No, [1] Yes.",
                std::pair<int, int>{ 0, 1 }
            },
            {
                Command::Argument::ARGTYPE::INT,
                "colours",
                "Whether to reset the draw and background colours. [0] No, [1] Yes.",
                std::pair<int, int>{ 0, 1 }
            }
        },

        .presets = {
            { "Reset Canvas", { 0, 1, 0, 0 } },
            { "Reset All",    { 1, 1, 1, 1 } }
        }
    },

    .inputMetadata = {
        .keyboard = {
            {
                inp::Keyboard_Metadata{ SDL_SCANCODE_C, 0 },
                inp::Keyboard_Metadata{ SDL_SCANCODE_R, 1 }
            }
        },

        .cli = inp::CLI_Metadata{
            "reset",
            R"(Reset application state.
Arguments are:
    [0] Action queue. [0] Keep, [1] Reset.
    [1] Canvas. [0] Keep, [1] Reset.
    [2] Cursor. [0] Keep, [1] Reset.
    [3] Colours. [0] Keep, [1] Reset.)"
        },

        .gui = {
            {
                GUI::GUI_Metadata{
                    GUI::HEADER::FILE,
                    "Reset Canvas",
                    GUI::FUNCTION_TYPE::BINARY,
                    0,
                    std::monostate{}
                },

                GUI::GUI_Metadata{
                    GUI::HEADER::FILE,
                    "Reset All (Includes undo/redo history)",
                    GUI::FUNCTION_TYPE::BINARY,
                    1,
                    std::monostate{}
                }
            }
        }
    },

    .processor = &LuxinPaint::processReset
},

    // ============================================================
    // UNDO / REDO
    // ============================================================

{
    .ID = COMMAND_ID::ACTION_HISTORY,

    .commandMetadata = {
        .commandDescription = "Navigate the action history.",

        .arguments = {
            {
                Command::Argument::ARGTYPE::INT,
                "direction",
                "History direction. [0] Undo, [1] Redo.",
                std::pair<int, int>{ 0, 1 }
            }
        },

        .presets = {
            { "Undo", { 0 } },
            { "Redo", { 1 } }
        }
    },

    .inputMetadata = {
        .keyboard = {
            {
                inp::Keyboard_Metadata{ SDL_SCANCODE_N, 0 },
                inp::Keyboard_Metadata{ SDL_SCANCODE_M, 1 }
            }
        },

        .cli = inp::CLI_Metadata{
            "history",
            R"(Navigate the action history.
Arguments are:
    [0] Direction. [0] Undo, [1] Redo.)"
        },

        .gui = {
            {
                GUI::GUI_Metadata{
                    GUI::HEADER::EDIT,
                    "Undo",
                    GUI::FUNCTION_TYPE::BINARY,
                    0,
                    std::monostate{}
                },

                GUI::GUI_Metadata{
                    GUI::HEADER::EDIT,
                    "Redo",
                    GUI::FUNCTION_TYPE::BINARY,
                    1,
                    std::monostate{}
                }
            }
        }
    },

    .processor = &Action::processHistory
},

    // ============================================================
    // CANVAS SIZE
    // ============================================================

{
    .ID = COMMAND_ID::CANVAS_RESIZE,

    .commandMetadata = {
        .commandDescription = "Resize the canvas.",

        .arguments = {
            {
                Command::Argument::ARGTYPE::INT,
                "width",
                "New canvas width.",
                std::pair<int, int>{
                    DEFAULT_CANVAS_WIDTH_MIN,
                    DEFAULT_CANVAS_WIDTH_MAX
                },
                false
            },
            {
                Command::Argument::ARGTYPE::INT,
                "height",
                "New canvas height.",
                std::pair<int, int>{
                    DEFAULT_CANVAS_HEIGHT_MIN,
                    DEFAULT_CANVAS_HEIGHT_MAX
                },
                false
            }
        },

        .presets = {
            {
                "Reset Size",
                {
                    DEFAULT_CANVAS_WIDTH_CUR,
                    DEFAULT_CANVAS_HEIGHT_CUR
                }
            },
            {
                "Set Width",
                {
                    DEFAULT_CANVAS_WIDTH_CUR,
                    std::monostate{}
                }
            },
            {
                "Set Height",
                {
                    std::monostate{},
                    DEFAULT_CANVAS_HEIGHT_CUR
                }
            }
        }
    },

    .inputMetadata = {
        .keyboard = {
            {
                inp::Keyboard_Metadata{
                    SDL_SCANCODE_7,
                    0
                }
            }
        },

        .cli = inp::CLI_Metadata{
            "resize",
            R"(Resize the canvas.
Arguments are:
    [0] {Optional} Width. Omit to leave unchanged.
    [1] {Optional} Height. Omit to leave unchanged.)"
        },

        .gui = {
            {
                GUI::GUI_Metadata{
                    GUI::HEADER::FILE,
                    "Adjust canvas width",
                    GUI::FUNCTION_TYPE::SLIDER,
                    1,

                    GUI::SLIDER_METADATA{
                        DEFAULT_CANVAS_WIDTH_MIN,
                        DEFAULT_CANVAS_WIDTH_MAX,
                        0,
                        GUI::Resolver::resolveCanvasWidth
                    }
                },

                GUI::GUI_Metadata{
                    GUI::HEADER::FILE,
                    "Adjust canvas height",
                    GUI::FUNCTION_TYPE::SLIDER,
                    2,

                    GUI::SLIDER_METADATA{
                        DEFAULT_CANVAS_HEIGHT_MIN,
                        DEFAULT_CANVAS_HEIGHT_MAX,
                        1,
                        GUI::Resolver::resolveCanvasHeight
                    }
                }
            }
        }
    },

    .processor = &Canvas::processCanvasSize
},
{
    .ID = COMMAND_ID::OBJECT_EDITOR,

    .commandMetadata = {
        .commandDescription = "Toggle object editing mode.",

        .arguments = {
            {
                Command::Argument::ARGTYPE::UNFIXED_TYPE,
                "object",
                "Optional object ID or object name.",
                std::nullopt,
                false
            }
        },

        .presets = {
            {
                "New Object",
                {
                    std::monostate{}
                }
            }
        }
    },

    .inputMetadata = {
            .cli = inp::CLI_Metadata{
    "object",
    R"(Toggle object editing mode.
Arguments are:
    [0] {Optional} Object ID or object name. Omit to create a new object.)"
},
        .gui = {
            {
                GUI::GUI_Metadata{
                    GUI::HEADER::TOOLS,
                    "Object Editor",
                    GUI::FUNCTION_TYPE::BINARY,
                    0
                }
            }
        }
    },

    .processor = &Object::toggleObjectEditMode
},
{
    .ID = COMMAND_ID::OBJECT_MOVE,

    .commandMetadata = {
        .commandDescription = "Move an object.",

        .arguments = {
            {
                Command::Argument::ARGTYPE::INT,
                "setting",
                "Movement mode. 0 = set coordinate, 1 = set to cursor, 2 = toggle stick-to-cursor.",
                std::pair<int, int>{ 0, 2 },
                true
            },
            {
                Command::Argument::ARGTYPE::COORDINATE,
                "destination",
                "Destination coordinate. Required only when setting == 0.",
                std::nullopt,
                false
            },
            {
                Command::Argument::ARGTYPE::UNFIXED_TYPE,
                "object",
                "Optional object ID or object name. Uses selected object if omitted.",
                std::nullopt,
                false
            }
        },

        .presets = {
            {
                "Set Position",
                {
                    0,
                    coordinate{ 0, 0 },
                    std::monostate{}
                }
            },
            {
                "Move To Cursor",
                {
                    1,
                    std::monostate{},
                    std::monostate{}
                }
            },
            {
                "Stick To Cursor",
                {
                    2,
                    std::monostate{},
                    std::monostate{}
                }
            }
        }
    },

    .inputMetadata = {
        .keyboard = {
            {
                inp::Keyboard_Metadata{
                    SDL_SCANCODE_8,
                    2,
                    true
                }
            }
        },
        .cli = inp::CLI_Metadata{
            "move_object",
            R"(Move an object.
Arguments are:
    [0] Setting. 0 = set coordinate, 1 = move to cursor, 2 = toggle stick-to-cursor.
    [1] {Optional} Destination coordinate. Used when setting == 0.
    [2] {Optional} Object ID or object name. Uses selected object if omitted.)"
        }
    },

    .processor = &Object::moveObject
},
{
    .ID = COMMAND_ID::OBJECT_SELECT,

    .commandMetadata = {
        .commandDescription = "Select an object.",

        .arguments = {
            {
                Command::Argument::ARGTYPE::INT,
                "selectionMode",
                "Selection mode. 0 = select object underneath cursor, 1 = select by identifier, 2 = deselect.",
                std::pair<int, int>{ 0, 2 },
                true
            },
            {
                Command::Argument::ARGTYPE::UNFIXED_TYPE,
                "object",
                "Object ID or object name. Required only when selectionMode == 1.",
                std::nullopt,
                false
            }
        },

        .presets = {
            {
                "Select Under Cursor",
                {
                    0,
                    std::monostate{}
                }
            },
            {
                "Select By Identifier",
                {
                    1,
                    std::monostate{}
                }
            },
            {
                "Deselect",
                {
                    2,
                    std::monostate{}
                }
            }
        }
    },

    .inputMetadata = {
        .keyboard = 
        {
            {
                inp::Keyboard_Metadata { SDL_SCANCODE_I, 0 }
            }
        },
        .cli = inp::CLI_Metadata{
            "select_object",
            R"(Select or deselect an object.
Arguments are:
    [0] Selection mode. 0 = select object underneath cursor, 1 = select by identifier, 2 = deselect.
    [1] {Optional} Object ID or object name. Required when selectionMode == 1.)"
        }
    },

    .processor = &Object::selectObject
},
} };