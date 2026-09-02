#include "Command Handler.h"
#include "Master Handler.h"

void Command_Handler::addCommand(Command_OLD command) {
    commandQueue.emplace_back(std::move(command));
}
void Command_Handler::constructCommand(COMMAND_ID command) {
    Command_Definition result = *COMMAND_ID_DEF_MAP[command];
    addCommand(result.command);
}
void Command_Handler::constructCommand(COMMAND_ID command, Command_OLD::Payload_OLD payload) {
    Command_Definition result = *COMMAND_ID_DEF_MAP[command];
    result.command.payload = payload;
    addCommand(result.command);
}

void Command_Handler::processCommand(Command_OLD& command) {

    auto& def = COMMAND_ID_DEF_MAP.at(command.ID);

    if (def->newProcessor) {
        def->newProcessor(MasterHandler, command);
        return;
    }

    COMMAND_PROCESSOR_ID pID = def->processor;

    switch (pID) {
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

    if (commandQueue.empty()) Action::commitCurrentAction(MasterHandler.ActionState);
    for (auto& command : commandQueue) processCommand(command);
    clearCommands();

}

size_t Command_Handler::commandQueueSize() { return commandQueue.size(); }
void Command_Handler::clearCommands() { commandQueue = {}; }


Command_Handler::Command_Handler(Master_Handler& mastH)
    : MasterHandler(mastH) {}