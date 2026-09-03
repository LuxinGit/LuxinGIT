#pragma once

#include "COMMAND.h"
#include <vector>

struct Master_Handler;

struct Command_Handler {

private:

    std::vector<Command_OLD> commandQueue;
    Master_Handler& MasterHandler;

    void addCommand(Command_OLD command);
    void processCommand(Command_OLD& command);

public:

    void processCommands();
    void constructCommand(COMMAND_ID command);
    void constructCommand(COMMAND_ID command, Command_OLD::Payload_OLD payload);

    size_t commandQueueSize();
    void clearCommands();

    Command_Handler(Master_Handler& mastH);
};
