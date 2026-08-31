#include <algorithm>

#include "Cursor Handler.h"
#include "Canvas Handler.h"

Cursor_Handler::Cursor_Handler(Canvas_Handler& varCanvH) : CanvasHandler(varCanvH) {
    if (ENABLE_MOUSE) penDown = false;
}

void Cursor_Handler::resetPoint(float& f, const int& width) {
    f = std::clamp(f, 0.0f, static_cast<float>(width - 1));
}
void Cursor_Handler::resetCursors() {
    if (!CanvasHandler.coordCheckDisplay(deltaCursor)) {
        resetPoint(deltaCursor.first, CanvasHandler.width); resetPoint(deltaCursor.second, CanvasHandler.width);
    }
    cursor = deltaCursor;
}

void Cursor_Handler::refreshCursor() {
    if (penDown && cursor != deltaCursor) CanvasHandler.DrawHandler.drawLine(cursor, deltaCursor, true);
    if (penContinuous) {
        penContinuous = false; penDown = false;
    }
    resetCursors();
}

void Cursor_Handler::processCommand(const Command& command) {
    switch (command.type) {
    case Command::TYPE::META:
        processMetaCommand(command);
        break;
    case Command::TYPE::MOVE:
        processMoveCommand(command);
        break;
    }
}

    void Cursor_Handler::processMetaCommand(const Command& command) {
        using Action = Command::META::ACTION;
        switch (static_cast<Action>(command.action)) {
        case Action::CHANGE_DRAWSTEP:
            processChangeDrawstepCommand(command);
            break;
        case Action::SAVE_ORIGIN:
            origin = cursor;
            break;
        }
    }
        void Cursor_Handler::processChangeDrawstepCommand(const Command& command) {
            using setting = Command::META::CHANGE_DRAWSTEP;
            switch (static_cast<setting>(command.setting)) {
            case setting::ADD_PAYLOAD:
                drawStep = std::clamp(drawStep + std::get<int>(command.payload), DEFAULT_DRAWSTEP_MIN, DEFAULT_DRAWSTEP_MAX);
                break;
            case setting::SET_TO_PAYLOAD:
                drawStep = std::clamp(std::get<int>(command.payload), DEFAULT_DRAWSTEP_MIN, DEFAULT_DRAWSTEP_MAX);
                break;
            }

        }

    void Cursor_Handler::processMoveCommand(const Command& command) {
        switch (static_cast<Command::MOVE::ACTION>(command.action)) {
        case Command::MOVE::ACTION::DIRECTION:
            processDirectionCommand(command);
            break;
        case Command::MOVE::ACTION::SET:
            processSetCommand(command);
            break;
        }
    }
        void Cursor_Handler::processDirectionCommand(const Command& command) {
            switch (static_cast<Command::MOVE::DIRECTION>(command.setting)) {
            case Command::MOVE::DIRECTION::UP:
                deltaCursor.second -= drawStep;
                break;
            case  Command::MOVE::DIRECTION::RIGHT:
                deltaCursor.first += drawStep;
                break;
            case  Command::MOVE::DIRECTION::DOWN:
                deltaCursor.second += drawStep;
                break;
            case  Command::MOVE::DIRECTION::LEFT:
                deltaCursor.first -= drawStep;
                break;
            }
        }
        void Cursor_Handler::processSetCommand(const Command& command) {
            switch (static_cast<Command::MOVE::SET>(command.setting)) {
            case Command::MOVE::SET::RESET_TO_ORIGIN:
                deltaCursor = origin;
                break;
            case Command::MOVE::SET::USE_PAYLOAD:
                deltaCursor = std::get<std::pair<float, float>>(command.payload);
                break;
            }
        }

