#include "Keyboard Handler.h"
#include "Command Handler.h"
#include "Master Handler.h"
#include "SDL3/SDL.h"

void Keyboard_Handler::harvestKeyboardState() {

    if (ImGui::GetIO().WantCaptureKeyboard) return;

    const bool* keyboardState = SDL_GetKeyboardState(nullptr);
    for (const auto& [scancode, id] : keyBindings) {
        if (keyboardState[scancode] &&
            (COMMAND_ID_DEF_MAP.at(id)->command.repeatable || !previousKeyboardState[scancode])) 
            CommandHandler.constructCommand(id); 

        previousKeyboardState[scancode] = keyboardState[scancode];
    }
}

Keyboard_Handler::Keyboard_Handler(Command_Handler& CommH) : CommandHandler(CommH) {}