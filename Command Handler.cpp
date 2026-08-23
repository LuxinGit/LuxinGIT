#include "Command Handler.h"
#include "Master Handler.h"

void Command_Handler::addCommand(Command command) {
    commandQueue.emplace_back(std::move(command));
}
void Command_Handler::constructCommand(COMMAND_ID command, Command::Payload payload) {
    Command_Definition result = *COMMAND_ID_DEF_MAP[command];
    result.command.payload = payload;
    addCommand(result.command);
}

void Command_Handler::processCommand(const Command& command) {

    COMMAND_PROCESSOR_ID pID = COMMAND_ID_DEF_MAP.at(command.ID)->processor;

    switch (pID) {
        case COMMAND_PROCESSOR_ID::CANVAS_HANDLER:
            MasterHandler.CanvasHandler.processCommand(command);
            break;

        case COMMAND_PROCESSOR_ID::CURSOR_HANDLER:
            MasterHandler.CanvasHandler.CursorHandler.processCommand(command);
            break;

        case COMMAND_PROCESSOR_ID::DRAW_HANDLER:
            MasterHandler.CanvasHandler.DrawHandler.processCommand(command);
            break;

        case COMMAND_PROCESSOR_ID::CLI_HANDLER:
            MasterHandler.CLIHandler.processCommand(command);
            break;

        case COMMAND_PROCESSOR_ID::KEYBOARD_HANDLER:
            // LAZY
            break;

        case COMMAND_PROCESSOR_ID::MOUSE_HANDLER:
            MasterHandler.MouseHandler.processCommand(command);
            break;
    }
}
void Command_Handler::processCommands() {
    for (const auto& command : commandQueue) processCommand(command);
    clearCommands();
}

size_t Command_Handler::commandQueueSize() { return commandQueue.size(); }
void Command_Handler::clearCommands() { commandQueue = {}; }


Command_Handler::Command_Handler(Master_Handler& mastH)
    : MasterHandler(mastH) {}