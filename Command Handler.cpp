#include "Command Handler.h"
#include "Master Handler.h"

void Command_Handler::addCommand(Command command) {
    commandQueue.emplace_back(std::move(command));
}
void Command_Handler::constructCommand(COMMAND command, Command::Payload payload) {
    Command result = commandMapping.at(command);
    result.payload = payload;
    addCommand(result);
}

void Command_Handler::processAppCommand(const Command& command) {
    using App = Command::APP;
    switch (static_cast<App::ACTION>(command.action)) {
    case App::ACTION::INPUT_MODE:
        switch (static_cast<App::INPUT_MODE>(command.setting)) {
        case App::INPUT_MODE::CLI:
            MasterHandler.CLIHandler.beginCLILoop();
            break;
        case App::INPUT_MODE::MOUSE:
            MasterHandler.MouseHandler.processMouseAppCommand();
            break;
        }
    }
}
void Command_Handler::processCommand(const Command& command) {
    switch (command.type) {
    case Command::TYPE::DRAW:
        MasterHandler.CanvasHandler.DrawHandler.processDrawCommand(command);
        break;
    case Command::TYPE::MOVE:
        MasterHandler.CanvasHandler.processMoveCommand(command);
        break;
    case Command::TYPE::META:
        MasterHandler.CanvasHandler.processMetaCommand(command);
        break;
    case Command::TYPE::APP:
        processAppCommand(command);
        break;
    }
}
void Command_Handler::processCommands() {
    for (const auto& command : commandQueue) processCommand(command);
    clearCommands();
}

int Command_Handler::commandQueueSize() { return commandQueue.size(); }
void Command_Handler::clearCommands() { commandQueue = {}; }

Command_Handler::Command_Handler(Master_Handler& mastH)
    : MasterHandler(mastH) {}