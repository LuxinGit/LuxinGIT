#include <algorithm>

#include "Cursor Handler.h"
#include "Canvas Handler.h"

Cursor_Handler::Cursor_Handler(Canvas_Handler& varCanvH) : CanvasHandler(varCanvH) {}

void Cursor_Handler::resetPoint(float& f, const int& width) {
    f = std::clamp(f, 0.0f, static_cast<float>(width - 1));
}
void Cursor_Handler::resetCursors() {
    cursor = CanvasHandler.addCoords(cursor, deltaCursor);
    deltaCursor = { 0 , 0 };
    if (!CanvasHandler.coordCheck(cursor)) {
        resetPoint(cursor.first, CanvasHandler.width); resetPoint(cursor.second, CanvasHandler.width);
    }
}

void Cursor_Handler::checkCursorLine() {
    if (penDown) CanvasHandler.DrawHandler.drawLine(cursor, CanvasHandler.addCoords(cursor, deltaCursor), true);
    resetCursors();
}

void Cursor_Handler::processChangeDrawstepCommand(const Command& command) {
    using setting = Command::META::CHANGE_DRAWSTEP;
    switch (static_cast<setting>(command.setting)) {
    case setting::ADD_PAYLOAD:
        drawStep = std::clamp(drawStep + std::get<int>(command.payload), 1, 50);
        break;
    case setting::SET_TO_PAYLOAD:
        drawStep = std::clamp(std::get<int>(command.payload), 1, 50);
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
        cursor = origin;
        break;
    case Command::MOVE::SET::USE_PAYLOAD:
        cursor = std::get<std::pair<float, float>>(command.payload);
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
