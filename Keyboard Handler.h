#pragma once

#include "Command.h"
#include "SDL3/SDL.h"

struct Command_Handler;

struct Keyboard_Handler {

private:

    std::unordered_map<SDL_Scancode, COMMAND> keyMapping = {
        { SDL_SCANCODE_W, COMMAND::MOVE_UP },
        { SDL_SCANCODE_S, COMMAND::MOVE_DOWN },
        { SDL_SCANCODE_A, COMMAND::MOVE_LEFT },
        { SDL_SCANCODE_D, COMMAND::MOVE_RIGHT },
        { SDL_SCANCODE_Q, COMMAND::MOVE_RESET },

        { SDL_SCANCODE_1, COMMAND::PEN_DOWN },

        { SDL_SCANCODE_G, COMMAND::DRAW_CIRCLE },
        { SDL_SCANCODE_L, COMMAND::DRAW_CIRCLE_RAINBOW },
        { SDL_SCANCODE_F, COMMAND::DRAW_FILL_DRAWCOLOUR},

        { SDL_SCANCODE_J, COMMAND::COLOUR_RANDOM },
        { SDL_SCANCODE_E, COMMAND::RAINBOW_DEFAULT },

        { SDL_SCANCODE_Z, COMMAND::DRAWSTEP_DECREASE },
        { SDL_SCANCODE_X, COMMAND::DRAWSTEP_INCREASE },

        { SDL_SCANCODE_V, COMMAND::PEN_WIDTH_DECREASE },
        { SDL_SCANCODE_B, COMMAND::PEN_WIDTH_INCREASE },

        { SDL_SCANCODE_C, COMMAND::RESET_CANVAS },
        { SDL_SCANCODE_9, COMMAND::RESET_CURSOR },
        { SDL_SCANCODE_2, COMMAND::MOVE_SAVE_ORIGIN },
        { SDL_SCANCODE_0, COMMAND::INPUT_CLI_ENABLE },
        { SDL_SCANCODE_3, COMMAND::INPUT_MOUSE_ENABLE },
    };

    Command_Handler& CommandHandler;
    std::array<bool, SDL_SCANCODE_COUNT> previousKeyboardState{};

public:

    void harvestKeyboardState();

    Keyboard_Handler(Command_Handler& CommH);

};
