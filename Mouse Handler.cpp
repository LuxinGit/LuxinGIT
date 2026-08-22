#include "Mouse Handler.h"
#include "SDL3/SDL.h"
#include "Command.h"
#include "Command Handler.h"

#include <unordered_map>
#include <vector>

std::unordered_map<SDL_MouseButtonFlags, COMMAND> mouseBindings = {
    { SDL_BUTTON_LMASK, COMMAND::PEN_DOWN }
};

void Mouse_Handler::harvestMouseState() {

    if (!enable_mouse) return;

    SDL_MouseButtonFlags mouseState = SDL_GetMouseState(&x, &y);

    for (const auto& [button, command] : mouseBindings) 
        if (mouseState & button) CommandHandler.constructCommand(command);

}

Mouse_Handler::Mouse_Handler(std::pair<float, float>& cursor, Command_Handler& varCommH)
    : x(cursor.first), y(cursor.second), CommandHandler(varCommH) {}

void Mouse_Handler::processMouseAppCommand() {

    enable_mouse = !enable_mouse;

    if (enable_mouse) SDL_HideCursor();
    else SDL_ShowCursor();

}