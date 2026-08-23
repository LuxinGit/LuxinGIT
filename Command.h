#pragma once

#include <variant>
#include <unordered_map>
#include <array>

#include "CONSTANTS.h"

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
                RESET = 0,
                CHANGE_COLOUR = 1,
                CHANGE_DRAWSTEP = 2,
                ENABLE_RAINBOW = 3,
                CHANGE_PEN_WIDTH = 4,
                PEN_DOWN = 5,
                SAVE_ORIGIN = 6
            };

            enum class RESET {
                RESET_CANVAS = 0,
                RESET_CURSOR = 1
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
            enum class ENABLE_RAINBOW {
                DEFAULT = 0,
                USE_PAYLOAD = 1
            };
            enum class CHANGE_PEN_WIDTH {
                ADD_PAYLOAD = 0,
                SET_TO_PAYLOAD = 1
            };
            enum class PEN_DOWN {
                DISCRETE = 0,
                CONTINUOUS = 1
            };
            enum class SAVE_ORIGIN {
                NORMAL = 0
            };

        private:

            META(ACTION varAction, int varSetting);

        public:

            ACTION action;
            int setting;

            META(RESET varSetting);
            META(CHANGE_COLOUR varSetting);
            META(CHANGE_DRAWSTEP varSetting);
            META(ENABLE_RAINBOW varSetting);
            META(CHANGE_PEN_WIDTH varSetting);
            META(PEN_DOWN varSetting);
            META(SAVE_ORIGIN varSetting);

        };
        struct APP {

            enum class ACTION {
                INPUT_MODE = 0,
            };

            enum class INPUT_MODE {
                CLI = 0,
                MOUSE = 1
            };

        private:

            APP(ACTION varAction, int varSetting = 0);

        public:

            APP(INPUT_MODE varSetting);

            ACTION action;
            int setting;

        };

    using Payload = std::variant<std::monostate, int, std::pair<float, float>, std::array<uint8_t, 4>>;

private:

    Command(TYPE vartype, int varaction, int varsetting, bool varRepeatable, Payload varPayload = {}); 

public:

    Command(MOVE cmd, bool repeatable, Payload payload = {});
    Command(DRAW cmd, bool repeatable, Payload payload = {});
    Command(META cmd, bool repeatable, Payload payload = {});
    Command(APP cmd, bool repeatable, Payload payload = {});

    TYPE type;
    int action; // differentiates between commands.
    int setting; // settings within commands.
    bool repeatable;
    Payload payload;

};

enum class COMMAND {
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

    RAINBOW_DEFAULT,
    RAINBOW_SET,

    DRAWSTEP_DECREASE,
    DRAWSTEP_INCREASE,
    DRAWSTEP_SET,

    PEN_DOWN,
    PEN_HELD_DOWN,
    PEN_WIDTH_DECREASE,
    PEN_WIDTH_INCREASE,
    PEN_SET,

    INPUT_CLI_ENABLE,
    INPUT_MOUSE_ENABLE,

    RESET_CANVAS,
    RESET_CURSOR,

    INVALID
};

inline static const std::unordered_map<COMMAND, Command> commandMapping = {
    // ANY CONSTANTS DEFINED HERE SHOULD USE constructCommand() TO PASS INFORMATION
    // CONSTANTS DEFINED HERE ARE SET FOR COMMANDS THAT MUST HAVE PAYLOAD INFORMATION TO RUN.
    { COMMAND::MOVE_UP,             Command{Command::MOVE::DIRECTION::UP, true} },
    { COMMAND::MOVE_DOWN,           Command{Command::MOVE::DIRECTION::DOWN, true} },
    { COMMAND::MOVE_LEFT,           Command{Command::MOVE::DIRECTION::LEFT, true} },
    { COMMAND::MOVE_RIGHT,          Command{Command::MOVE::DIRECTION::RIGHT, true} },
    { COMMAND::MOVE_RESET,          Command{Command::MOVE::SET::RESET_TO_ORIGIN, false} },
    { COMMAND::MOVE_SET_POINT,      Command{Command::MOVE::SET::USE_PAYLOAD, false, DEFAULT_CURSOR_POINT} },

    { COMMAND::DRAW_CIRCLE,         Command{Command::DRAW::CIRCLE::NORMAL, false} },
    { COMMAND::DRAW_CIRCLE_RAINBOW, Command{Command::DRAW::CIRCLE::RAINBOW, false} },
    { COMMAND::DRAW_FILL_PAYLOAD,   Command{Command::DRAW::FILL::USE_PAYLOAD, false} },
    { COMMAND::DRAW_FILL_DRAWCOLOUR,Command{Command::DRAW::FILL::USE_DRAW_COLOUR, false} },

    { COMMAND::COLOUR_RESET,        Command{Command::META::CHANGE_COLOUR::DEFAULT, false} },
    { COMMAND::COLOUR_RANDOM,       Command{Command::META::CHANGE_COLOUR::RANDOM, false} },
    { COMMAND::COLOUR_SET,          Command{Command::META::CHANGE_COLOUR::USE_PAYLOAD, false, DEFAULT_DRAW_COLOUR} },

    { COMMAND::RAINBOW_DEFAULT,     Command{Command::META::ENABLE_RAINBOW::DEFAULT, false} },
    { COMMAND::RAINBOW_SET,         Command{Command::META::ENABLE_RAINBOW::USE_PAYLOAD, false, 1000} },

    { COMMAND::DRAWSTEP_DECREASE,   Command{Command::META::CHANGE_DRAWSTEP::ADD_PAYLOAD, false, -1} },
    { COMMAND::DRAWSTEP_INCREASE,   Command{Command::META::CHANGE_DRAWSTEP::ADD_PAYLOAD, false,  1} },
    { COMMAND::DRAWSTEP_SET,        Command{Command::META::CHANGE_DRAWSTEP::SET_TO_PAYLOAD, false,  1} },


    { COMMAND::PEN_DOWN,            Command{Command::META::PEN_DOWN::DISCRETE, false} },
    { COMMAND::PEN_HELD_DOWN,       Command{Command::META::PEN_DOWN::CONTINUOUS, true} },
    { COMMAND::PEN_WIDTH_DECREASE,  Command{Command::META::CHANGE_PEN_WIDTH::ADD_PAYLOAD, false, -1} },
    { COMMAND::PEN_WIDTH_INCREASE,  Command{Command::META::CHANGE_PEN_WIDTH::ADD_PAYLOAD, false, 1} },
    { COMMAND::PEN_SET,             Command{Command::META::CHANGE_PEN_WIDTH::SET_TO_PAYLOAD, false, 1} },

    { COMMAND::RESET_CANVAS,        Command{Command::META::RESET::RESET_CANVAS, false} },
    { COMMAND::RESET_CURSOR,        Command{Command::META::RESET::RESET_CURSOR, false} },
    { COMMAND::MOVE_SAVE_ORIGIN,    Command{Command::META::SAVE_ORIGIN::NORMAL, false} },

    { COMMAND::INPUT_CLI_ENABLE,    Command{Command::APP::INPUT_MODE::CLI, false} },
    { COMMAND::INPUT_MOUSE_ENABLE,  Command{Command::APP::INPUT_MODE::MOUSE, false} },

};

