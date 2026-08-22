#include "Keyboard Handler.h"
#include "Command Handler.h"
#include "Master Handler.h"
#include "SDL3/SDL.h"

void Keyboard_Handler::harvestKeyboardState() {
    const bool* keyboardState = SDL_GetKeyboardState(nullptr);
    for (const auto& [scancode, binding] : keyMapping) {
        Command comm = commandMapping.at(binding);
        if (keyboardState[scancode] &&
            (comm.repeatable || !previousKeyboardState[scancode])) CommandHandler.addCommand(comm); // NOT USING CONSTRUCTCOMMAND AS KEYBOARD INPUTS CARRY NO ADDITIONAL DATA

        previousKeyboardState[scancode] = keyboardState[scancode];
    }
}

Keyboard_Handler::Keyboard_Handler(Command_Handler& CommH) : CommandHandler(CommH) {}