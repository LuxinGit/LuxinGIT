#include "Mouse Handler.h"
#include "Master Handler.h"

#include <vector>



void Mouse_Handler::harvestMouseState() {

    if (!enableMouse) return;
    if (ImGui::GetIO().WantCaptureMouse) {
        SDL_ShowCursor();
        return;
    }
    SDL_MouseButtonFlags mouseState = SDL_GetMouseState(&x, &y);

    for (const auto& [button, command] : mouseBindings) 
        if (mouseState & button) MasterHandler.CommandHandler.constructCommand(command);

}

Mouse_Handler::Mouse_Handler(std::pair<float, float>& cursor, Master_Handler& varMasH)
    : x(cursor.first), y(cursor.second), MasterHandler(varMasH) {}

void Mouse_Handler::processCommand(const Command_OLD& command) {
    switch (command.type) {
    case Command_OLD::TYPE::APP:
        processAppCommand(command);
        break;
    }
}

    void Mouse_Handler::processAppCommand(const Command_OLD& command) {

        using action = Command_OLD::APP::ACTION;
        switch (static_cast<action>(command.action)) {
        case (action::INPUT_MODE):
            switch (static_cast<Command_OLD::APP::INPUT_MODE>(command.setting)) {
            case (Command_OLD::APP::INPUT_MODE::MOUSE):
                processMouseCommand(command);
                break;
            }
        }
    }
        void Mouse_Handler::processMouseCommand(const Command_OLD& command) {
            enableMouse = !enableMouse;
        }