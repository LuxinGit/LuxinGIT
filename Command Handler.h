#pragma once

#include "Command.h"
#include <vector>

struct Master_Handler;

struct Command_Handler {

private:

    std::vector<Command> commandQueue;
    Master_Handler& MasterHandler;

    void addCommand(Command command);
    void processCommand(const Command& command);

public:

    void processCommands();
    void constructCommand(COMMAND_ID command);
    void constructCommand(COMMAND_ID command, Command::Payload payload);

    size_t commandQueueSize();
    void clearCommands();

    Command_Handler(Master_Handler& mastH);
};

