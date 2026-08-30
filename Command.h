#pragma once

#include <variant>
#include <unordered_map>
#include <array>
#include <optional>
#include <string>

#include <SDL3/SDL.h>

#include "CONSTANTS.h"

struct Master_Handler;

enum class COMMAND_ID {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_RESET,
    MOVE_SET_POINT,
    MOVE_SAVE_ORIGIN,

    DRAW_CIRCLE,
    DRAW_CIRCLE_RAINBOW,
    DRAW_POINT,

    DRAW_FILL_PAYLOAD,
    DRAW_FILL_DRAWCOLOUR,

    COLOUR_RESET,
    COLOUR_RANDOM,
    COLOUR_SET,

    PENMODE_DRAW,
    PENMODE_RUBBER,
    PENMODE_RAINBOW,

    DRAWSTEP_DECREASE,
    DRAWSTEP_INCREASE,
    DRAWSTEP_SET,

    UNDO,
    REDO,

    PEN_DOWN,
    PEN_HELD_DOWN,
    PEN_WIDTH_DECREASE,
    PEN_WIDTH_INCREASE,
    PEN_SET,

    INPUT_CLI_ENABLE,
    INPUT_MOUSE_ENABLE,

    RESET_CANVAS,
    RESET_ACTION_QUEUE,
    RESET_CURSOR,
    RESET_ALL,

    INVALID
};

struct Command {

public:

    enum class TYPE {
        DRAW = 0,
        MOVE = 1,
        META = 2,
        APP = 3
    };
        struct MOVE {

            enum class ACTION {
                DIRECTION = 0,
                SET = 1
            };

            enum class DIRECTION {
                RIGHT = 0,
                DOWN = 1,
                LEFT = 2,
                UP = 3
            };
            enum class SET {
                RESET_TO_ORIGIN = 0,
                USE_PAYLOAD = 1
            };

        private:

            MOVE(ACTION varAction, int varSetting = 0);

        public:

            ACTION action;
            int setting;

            MOVE(DIRECTION varSetting);
            MOVE(SET varSetting);

        };
        struct DRAW {

            enum class ACTION {
                LINE = 0,
                CIRCLE = 1,
                POINT = 2,
                FILL = 10
            };

            enum class LINE {
                NORMAL = 0
            };
            enum class CIRCLE {
                NORMAL = 0,
                RAINBOW = 1
            };
            enum class POINT {
                USE_PAYLOAD = 0,
                USE_CURSOR = 1
            };

            enum class FILL {
                USE_PAYLOAD = 0,
                USE_DRAW_COLOUR = 1
            };

        private:

            DRAW(ACTION varAction, int varSetting = 0);

        public:

            ACTION action;
            int setting;

            DRAW(LINE varSetting);
            DRAW(CIRCLE varSetting);
            DRAW(POINT varSetting);
            DRAW(FILL varSetting);
        };
        struct META {

            enum class ACTION {
                RESET,
                CHANGE_COLOUR,
                CHANGE_DRAWSTEP,
                CHANGE_PENMODE,
                CHANGE_PEN_WIDTH,
                SAVE_ORIGIN,
            };

            enum class RESET {
                RESET_CANVAS = 0,
                RESET_CURSOR = 1,
                RESET_ACTION_QUEUE = 2,
                RESET_ALL = 3,
            };
            enum class CHANGE_COLOUR {
                DEFAULT = 0,
                USE_PAYLOAD = 1,
                RANDOM = 2
            };
            enum class CHANGE_DRAWSTEP {
                ADD_PAYLOAD = 0,
                SET_TO_PAYLOAD = 1
            };
            enum class CHANGE_PENMODE {
                DRAW = 0,
                PEN_DOWN = 1,
                RUBBER = 2,
                RAINBOW = 3
            };
            enum class CHANGE_PEN_WIDTH {
                ADD_PAYLOAD = 0,
                SET_TO_PAYLOAD = 1
            };
            enum class SAVE_ORIGIN {
                NORMAL = 0
            };

            enum class PENMODE_PDOWN_PINTERP {
                DISCRETE = 0,
                CONTINUOUS = 1
            };

        private:

            META(ACTION varAction, int varSetting);

        public:

            ACTION action;
            int setting;

            META(RESET varSetting);
            META(CHANGE_COLOUR varSetting);
            META(CHANGE_DRAWSTEP varSetting);
            META(CHANGE_PENMODE varSetting);
            META(CHANGE_PEN_WIDTH varSetting);
            META(SAVE_ORIGIN varSetting);

        };
        struct APP {

            enum class ACTION {
                INPUT_MODE = 0,
                UNDO_REDO
            };

            enum class INPUT_MODE {
                CLI = 0,
                MOUSE = 1
            };

            enum class UNDO_REDO {
                UNDO = 0,
                REDO = 1
            };

        private:

            APP(ACTION varAction, int varSetting = 0);

        public:

            APP(INPUT_MODE varSetting);
            APP(UNDO_REDO varSetting);

            ACTION action;
            int setting;

        };

    using Payload = std::variant<std::monostate, int, std::pair<float, float>, std::array<uint8_t, 4>>;

private:

    Command(COMMAND_ID varID, TYPE vartype, int varaction, int varsetting, bool varRepeatable, Payload varPayload = {});

public:

    Command(COMMAND_ID varID, MOVE cmd, bool repeatable, Payload payload = {});
    Command(COMMAND_ID varID, DRAW cmd, bool repeatable, Payload payload = {});
    Command(COMMAND_ID varID, META cmd, bool repeatable, Payload payload = {});
    Command(COMMAND_ID varID, APP cmd, bool repeatable, Payload payload = {});

    TYPE type;
    int action; // differentiates between commands.
    int setting; // settings within commands.
    bool repeatable;
    Payload payload;

    COMMAND_ID ID;

};

enum class COMMAND_PROCESSOR_ID {
    CANVAS_HANDLER,
    CURSOR_HANDLER,
    DRAW_HANDLER,
    ACTION_HANDLER,
    CLI_HANDLER,
    KEYBOARD_HANDLER,
    MOUSE_HANDLER
};

struct GUI_METADATA {
    enum class HEADER {
        FILE = 0,
        EDIT = 1,
        TOOLS = 2,
        COUNT
    };

    enum class FUNCTION_TYPE {
        BINARY,
        SLIDER
    };

    struct SLIDER_METADATA {
        int minimum;
        int maximum;
        using Resolver = int* (*)(Master_Handler&);
        Resolver getUnderlying;
        int* underlying = nullptr; // used as reference for value, such that slider knows where we're at with whatever we're talking about.
        static int* resolvePenWidth(Master_Handler&);
        static int* resolveDrawstep(Master_Handler&);
    };

    HEADER header;
    FUNCTION_TYPE functionType;
    std::string_view label;
    std::variant<std::monostate, SLIDER_METADATA> metadata;

    GUI_METADATA(HEADER varH, std::string_view varL) :
        header(varH), functionType(FUNCTION_TYPE::BINARY), label(varL) {}
    GUI_METADATA(HEADER varH, std::string_view varL, SLIDER_METADATA varMD)
        : header(varH), functionType(FUNCTION_TYPE::SLIDER), label(varL), metadata(varMD) {}
};

struct Command_Definition {

    Command command;
    COMMAND_PROCESSOR_ID processor;

    std::optional<SDL_Scancode> keyBinding;
    std::optional<SDL_MouseButtonFlags> mouseBinding;
    std::optional<std::string_view> cliBinding;
    std::optional<GUI_METADATA> guiBinding;

};

inline std::unordered_map<COMMAND_ID, const Command_Definition*> COMMAND_ID_DEF_MAP = {};

inline static const auto COMMAND_REPO = std::to_array<Command_Definition>({

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
        COMMAND_PROCESSOR_ID::CANVAS_HANDLER,
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
        Command{COMMAND_ID::COLOUR_SET, Command::META::CHANGE_COLOUR::USE_PAYLOAD, false, DEFAULT_DRAW_COLOUR},
        COMMAND_PROCESSOR_ID::DRAW_HANDLER,
        std::nullopt,
        std::nullopt,
        "colour",
        std::nullopt
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
        std::nullopt
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
        }
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
        }
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
        }
    }

    });
/* TO ADD COMMANDS
* 
*   1. Create COMMAND_ID. This should describe what the action does.
*   2. As necessary, create Command logic (e.g Command::Draw::Circle)
*   3. Add to COMMAND_REPO. Ensure COMMAND_PROCESSOR_ID matches correct processor.
* 
*   BELOW ARE SOME TEMPLATES FOR MAKING NEW processCommands.  
* 
* 
* void process__C__Command(const Command& command) {
    
    using T = Command::TYPE;
    
    switch (command.type) {
    case T::A:
        process__A__Command(command);
        break;
    }
    
}
void process__T__Command(const Command& command) {

    using A = Command::T::ACTION;

    switch (static_cast<A>(command.action)) {
    case (A::INPUT_MODE):
            process__A__Command(command);
            break;
        }
    }
void process__A__Command(const Command& command) {

    using S = Command::T::A

    switch (static_cast<S>(command.setting)) {
    case (S::SETTING_MODE):
        process__S__Command(command);
        break;
    }
}
* 
*   
* 
* 
* 
*/