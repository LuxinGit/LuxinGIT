#include "Mouse Handler.h"
#include "Command Handler.h"

#include <vector>



void Mouse_Handler::harvestMouseState() {

    if (!enable_mouse) return;

    SDL_MouseButtonFlags mouseState = SDL_GetMouseState(&x, &y);

    for (const auto& [button, command] : mouseBindings) 
        if (mouseState & button) CommandHandler.constructCommand(command);

}

Mouse_Handler::Mouse_Handler(std::pair<float, float>& cursor, Command_Handler& varCommH)
    : x(cursor.first), y(cursor.second), CommandHandler(varCommH) {
    if (enable_mouse) SDL_HideCursor();
    
}



void Mouse_Handler::processCommand(const Command& command) {
    switch (command.type) {
    case Command::TYPE::APP:
        processAppCommand(command);
        break;
    }
}

    void Mouse_Handler::processAppCommand(const Command& command) {

        using action = Command::APP::ACTION;
        switch (static_cast<action>(command.action)) {
        case (action::INPUT_MODE):
            switch (static_cast<Command::APP::INPUT_MODE>(command.setting)) {
            case (Command::APP::INPUT_MODE::MOUSE):
                processMouseCommand(command);
                break;
            }
        }
    }
        void Mouse_Handler::processMouseCommand(const Command& command) {
            enable_mouse = !enable_mouse;

            if (enable_mouse) SDL_HideCursor();
            else SDL_ShowCursor();
        }