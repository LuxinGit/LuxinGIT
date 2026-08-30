#include "CLI Handler.h"
#include "Master Handler.h"

#include <iostream>

std::string CLI_Handler::harvestInput(const std::string& Question, bool linebreak) {
    std::cout << Question << std::endl;
    std::string output;
    std::cin >> output;
    if (linebreak) std::cout << std::endl;
    return output;
}

std::pair<float, float> CLI_Handler::convertFP(const std::string& input) {
    std::pair<float, float> ret;
    std::string s = "";
    for (char c : input) {
        switch (c) {
        case '{':
            continue;
            break;
        case ',':
            ret.first = (std::stof(s));
            s = "";
            break;
        case '}':
            ret.second = (std::stof(s));
            break;
        default:
            s += c;
        }
    }
    return ret;
}
coordinate CLI_Handler::convertCoord(const std::string& input) {
    coordinate ret{ 100,100 };
    std::string s = "";
    for (char c : input) {
        switch (c) {
        case '{':
            continue;
            break;
        case ',':
            ret.x = (std::stoi(s));
            s = "";
            break;
        case '}':
            ret.y = (std::stoi(s));
            break;
        default:
            s += c;
        }
    }
    return ret;
}
std::array<uint8_t, 4> CLI_Handler::convertColour(const std::string& input) {
    std::array<uint8_t, 4> ret = { 200, 200, 200, 255 };
    int i = 0;
    std::string s = "";
    for (char c : input) {
        switch (c) {
        case '{':
            continue;
            break;
        case '}':
            [[fallthrough]];
        case ',':
            ret[i] = static_cast<uint8_t>(std::stoi(s));
            s = "";
            i++;
            break;
        default:
            s += c;
        }
    }
    return ret;
}

Command::Payload CLI_Handler::harvestPayload() {
    std::string input = harvestInput(R"(
Please input data for payload: 

    Integers should be input as such: 2
    Coordinates should be input as such: {100,100}
    Colours should be input as such: {200,200,200,255}")");

    switch (std::stoi(harvestInput(R"(
Please enter the payload's type:

    Integer:                [1]
    Coordinate:             [2]
    Colour:                 [3]
    FloatPair {depreciated} [4]

Or enter anything else to cancel)")))
    {
    case 1:
        return std::stoi(input);
    case 2:
        return convertCoord(input);
    case 3:
        return convertColour(input);
    case 4:
        return convertFP(input);
    default:
        return {};

    }

}

void CLI_Handler::createCommand() {

    // NO INPUT CHECKS.  WE DIE LIKE REAL MEN.

    COMMAND_ID command = COMMAND_ID::INVALID;

    std::string commandName = harvestInput("Please enter the name of the command: ");
    if (stringMapping.contains(commandName)) command = stringMapping.at(commandName);
    else return;

    if (!(harvestInput("Add Payload? [Y]") == "Y")) MasterHandler.CommandHandler.constructCommand(command);
    else MasterHandler.CommandHandler.constructCommand(command, harvestPayload());

};
void CLI_Handler::CLI_Loop() {
    while (true) {
        std::cout << R"(
You are at the beginning of the CLI loop.

Options:

    [1] Add command.
    [2] Execute current commands.
    [3] Quit.

)" << std::endl;
        switch (std::stoi(harvestInput(""))) {
        case 1:
            createCommand();
            break;
        case 2:
            if (harvestInput("Reset Cursor after execution? [Y]") == "Y") MasterHandler.CommandHandler.constructCommand(COMMAND_ID::RESET_CURSOR);
            MasterHandler.CommandHandler.processCommands();
            break;
        case 3:
            if (MasterHandler.CommandHandler.commandQueueSize()) if (harvestInput("You have unexecuted commands.  Continue? [Y]") != "Y") break;
            MasterHandler.CommandHandler.clearCommands();
            return;
        default:
            continue;
        }
    }
}
void CLI_Handler::beginCLILoop() {
    MasterHandler.CommandHandler.clearCommands();
    CLI_Loop();
}

void CLI_Handler::processCommand(const Command& command) {
    switch (command.type) {
    case Command::TYPE::APP:
        processAppCommand(command);
        break;
    }
}

    void CLI_Handler::processAppCommand(const Command& command) {

        using action = Command::APP::ACTION;
        switch (static_cast<action>(command.action)) {
        case (action::INPUT_MODE):
            switch (static_cast<Command::APP::INPUT_MODE>(command.setting)) {
            case (Command::APP::INPUT_MODE::CLI):
                processCLICommand(command);
                break;
            }
        }
    }
        void CLI_Handler::processCLICommand(const Command& command) {
            beginCLILoop();
        }

CLI_Handler::  CLI_Handler(Master_Handler& mastH) : MasterHandler(mastH) {}

