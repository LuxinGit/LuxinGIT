#pragma once
#pragma region Command Handler

#include "Command.h"
#include <vector>

struct Master_Handler;

struct Command_Handler {

private:

    std::vector<Command> commandQueue;
    Master_Handler& MasterHandler;

public:

    void addCommand(Command command);
    void constructCommand(COMMAND command, Command::Payload payload = {});

private:
    void processCommand(const Command& command);
public:
    void processCommands();

    int commandQueueSize();
    void clearCommands();

    Command_Handler(Master_Handler& mastH);
};

#pragma endregion
