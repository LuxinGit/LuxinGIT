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

void Command_Handler::processCommand(const Command& command) {
    switch (command.type) {
    case Command::TYPE::DRAW:
        MasterHandler.CanvasHandler.processDrawCommand(command);
        break;
    case Command::TYPE::MOVE:
        MasterHandler.CanvasHandler.processMoveCommand(command);
        break;
    case Command::TYPE::META:
        MasterHandler.CanvasHandler.processMetaCommand(command);
        break;
    case Command::TYPE::APP:
        MasterHandler.CLIHandler.beginCLILoop();
        break;
    }
}
void Command_Handler::processCommands() {
    for (const auto& command : commandQueue) processCommand(command);
    MasterHandler.CanvasHandler.refreshCursor();
    MasterHandler.SDLHandler.refreshPresent(MasterHandler.CanvasHandler.canvas, MasterHandler.CanvasHandler.width);
    clearCommands();
}

int Command_Handler::commandQueueSize() { return commandQueue.size(); }
void Command_Handler::clearCommands() { commandQueue = {}; }

Command_Handler::Command_Handler(Master_Handler& mastH)
    : MasterHandler(mastH) {}