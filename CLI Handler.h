#pragma once

#include <unordered_map>
#include <string>

#include "Command.h"
struct Master_Handler;

struct CLI_Handler {
private:

    Master_Handler& MasterHandler;
    bool enable_CLI = ENABLE_CLI;

    inline static const std::unordered_map<std::string, COMMAND> stringMapping = {
    { "up",              COMMAND::MOVE_UP },
    { "down",            COMMAND::MOVE_DOWN },
    { "left",            COMMAND::MOVE_LEFT },
    { "right",           COMMAND::MOVE_RIGHT },
    { "reset",           COMMAND::MOVE_RESET },
    { "move",            COMMAND::MOVE_SET_POINT },
    { "save_origin",     COMMAND::MOVE_SAVE_ORIGIN },

    { "circle",          COMMAND::DRAW_CIRCLE },
    { "rainbow_circle",  COMMAND::DRAW_CIRCLE_RAINBOW },

    { "default_colour",  COMMAND::COLOUR_RESET },
    { "random_colour",   COMMAND::COLOUR_RANDOM },
    { "colour",          COMMAND::COLOUR_SET },

    { "rainbow",         COMMAND::RAINBOW_SET },

    { "step_down",       COMMAND::DRAWSTEP_DECREASE },
    { "step_up",         COMMAND::DRAWSTEP_INCREASE },
    { "step_set",        COMMAND::DRAWSTEP_SET },

    { "pen",             COMMAND::PEN_DOWN },
    { "pen_thinner",     COMMAND::PEN_WIDTH_DECREASE },
    { "pen_thicker",     COMMAND::PEN_WIDTH_INCREASE },
    { "pen_set",         COMMAND::PEN_SET },

    { "reset_canvas",    COMMAND::RESET_CANVAS },
    { "reset_cursor",    COMMAND::RESET_CURSOR }
    };

    std::string harvestInput(const std::string& Question, bool linebreak = true);

    std::pair<float, float> convertCoord(const std::string& input);
    std::array<uint8_t, 4> convertColour(const std::string& input);

    Command::Payload harvestPayload();

    void createCommand();
    void CLI_Loop();

public:

    void beginCLILoop();

    CLI_Handler(Master_Handler& mastH);

};
