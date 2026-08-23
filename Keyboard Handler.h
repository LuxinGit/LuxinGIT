#pragma once

#include "Command.h"
#include "SDL3/SDL.h"

struct Command_Handler;

struct Keyboard_Handler {

private:

    Command_Handler& CommandHandler;
    std::array<bool, SDL_SCANCODE_COUNT> previousKeyboardState{};

public:

    std::unordered_map<SDL_Scancode, COMMAND_ID> keyBindings{};

    void harvestKeyboardState();

    Keyboard_Handler(Command_Handler& CommH);

};
