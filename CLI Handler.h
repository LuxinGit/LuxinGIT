#pragma once

#include <unordered_map>
#include <string>

#include "COMMAND.h"
struct Master_Handler;

struct CLI_Handler {
private:

    Master_Handler& MasterHandler;
    bool enable_CLI = ENABLE_CLI;

    std::string harvestInput(const std::string& Question, bool linebreak = true);

    std::pair<float, float> convertFP(const std::string& input);
    coordinate              convertCoord(const std::string& input);
    std::array<uint8_t, 4>  convertColour(const std::string& input);

    Command::Payload harvestPayload();

    void createCommand();
    void CLI_Loop();

public:

    std::unordered_map<std::string, COMMAND_ID> stringMapping = {};

    void beginCLILoop();

    CLI_Handler(Master_Handler& mastH);


    void processCommand(const Command& command);

    private:
        void processAppCommand(const Command& command);
            void processCLICommand(const Command& command);

};
