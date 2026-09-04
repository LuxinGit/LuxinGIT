#pragma once

#include <variant>
#include <unordered_map>
#include <array>
#include <optional>
#include <string>

#include <SDL3/SDL.h>

#include "STRUCTS.h"

#include <cassert>


struct Master_Handler;

enum class COMMAND_ID {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_RESET,
    MOVE_SET_POINT,
    MOVE_SAVE_ORIGIN,

    DRAW_CIRCLE,

    DRAW_FILL_PAYLOAD,
    DRAW_FILL_DRAWCOLOUR,

    CANVAS_RESIZE_SET_PAYLOAD,
    CANVAS_RESIZE_SET_WIDTH,
    CANVAS_RESIZE_SET_HEIGHT,

    COLOUR_SET_DEFAULT,
    COLOUR_SET_RANDOM,
    COLOUR_SET_DRAW,
    COLOUR_SET_BACKGROUND,
    COLOUR_SET_PICK,

    PENMODE_DRAW,
    PENMODE_RUBBER,
    PENMODE_RAINBOW,

    DRAWSTEP_DECREASE,
    DRAWSTEP_INCREASE,
    DRAWSTEP_SET,

    UNDO,
    REDO,

    PEN_DOWN,
    PEN_HELD_DOWN,
    PEN_WIDTH_DECREASE,
    PEN_WIDTH_INCREASE,
    PEN_SET,

    INPUT_CLI_ENABLE,
    INPUT_MOUSE_ENABLE,

    RESET_CANVAS,
    RESET_ACTION_QUEUE,
    RESET_CURSOR,
    RESET_ALL,

    INVALID
};

namespace Command {

    using argument = std::variant<
        int, 
        colour, 
        coordinate>;

    enum class ARGTYPE {
        INT = 0,
        COLOUR = 1,
        COORDINATE = 2
    };

    struct Argument_Definition {
        ARGTYPE type;
        std::string_view name;
        std::string_view desc;
        std::optional<std::pair<argument, argument>> constraints;
    };

    struct Argument_Metadata {
        std::vector<Argument_Definition> arguments;
        std::vector<argument> defaultArgs;
    };

    struct Command_Metadata {

        std::string_view                commandDescription;
        Argument_Metadata                 argumentMetadata;
        bool                              undoable = false;

    };

    struct Keyboard_Metadata {

        SDL_Scancode    SCANCODE;
        bool            repeatable;

    };

    struct CLI_Metadata {

        std::string_view    commandName;
        std::string_view    help;
    };

    struct Mouse_Metadata {

        SDL_MouseButtonFlags    MOUSECODE;

    };

    enum class HEADER {
        FILE,
        EDIT,
        TOOLS,
        COUNT
    };

    struct SLIDER_METADATA {
        int minimum;
        int maximum;

        using Resolver = int& (*)(Master_Handler&);
        Resolver resolve;
    };

    struct COLOUR_METADATA {
        using Resolver = colour & (*)(Master_Handler&);
        Resolver resolve;
    };

    enum class GUI_FUNCTION_TYPE {
        BINARY,
        SLIDER,
        COLOUR
    };

    using GUI_TYPE_METADATA = std::variant<std::monostate, SLIDER_METADATA, COLOUR_METADATA>;

    struct GUI_Metadata {

        HEADER            header;
        std::string_view  label;
        GUI_FUNCTION_TYPE TYPE;
        GUI_TYPE_METADATA typeMetadata;

    };

    struct Input_Metadata {

        std::optional<Keyboard_Metadata>    keyboard;
        std::optional<Mouse_Metadata>       mouse;
        std::optional<CLI_Metadata>         cli;
        std::optional<GUI_Metadata>         gui;

    };

    struct Cmd;

    using Command_Processor = void(*)(Master_Handler&, Cmd&);

    struct Cmd {

        COMMAND_ID                     ID;
        std::vector<argument>        args;
        Command_Processor       processor;

    };

    struct Command_Definition {

        COMMAND_ID                       ID;
        Command_Metadata    commandMetadata;
        Input_Metadata        inputMetadata;

        Command_Processor         processor;

    };

}

namespace Command::GUI_Resolver {
    int& resolvePenWidth(Master_Handler&);
    int& resolveDrawstep(Master_Handler&);
    int& resolveCanvasHeight(Master_Handler&);
    int& resolveCanvasWidth(Master_Handler&);
    colour& resolveDrawColourChange(Master_Handler&);
    colour& resolveBackgroundColourChange(Master_Handler&);
}

struct Input_State;

namespace Command::Processor {

    enum class returnCode {
        SUCCESS,
        ARG_COUNT_INVALID,
        ARG_TYPE_INVALID,
        ARG_RANGE_INVALID
    };

    std::pair<int, returnCode> constructCommand(Input_State& s, const Command_Definition* def, std::vector<argument> args = {});
    void processCommands(Master_Handler&);
}