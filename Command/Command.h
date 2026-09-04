#pragma once

#include <variant>
#include <array>
#include <optional>
#include <string>

#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_mouse.h>

#include "Application/STRUCTS.h"


struct Application_State;



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
        std::monostate,
        int,
        colour,
        coordinate>;

    struct Cmd;

}

struct Command_State {

    std::vector<Command::Cmd> commandQueue;

};

namespace Command::Argument {

    enum class ARGTYPE {
        INT = 1,
        COLOUR = 2,
        COORDINATE = 3
    };

    struct Argument_Definition {
        ARGTYPE type;
        std::string_view name;
        std::string_view desc;
        std::optional<std::pair<argument, argument>> constraints;
        bool required = true;
    };

}

namespace Command::Definition::Input {

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

}

namespace Command::Definition::Input::GUI {

    enum class HEADER {
        FILE,
        EDIT,
        TOOLS,
        COUNT
    };

    struct SLIDER_METADATA {
        int minimum;
        int maximum;

        using Resolver = int& (*)(Application_State&);
        Resolver resolve;
    };

    struct COLOUR_METADATA {
        using Resolver = colour & (*)(Application_State&);
        Resolver resolve;
    };

    enum class FUNCTION_TYPE {
        BINARY,
        SLIDER,
        COLOUR
    };

    using GUI_TYPE_METADATA = std::variant<std::monostate, SLIDER_METADATA, COLOUR_METADATA>;

    struct GUI_Metadata {

        HEADER            header;
        std::string_view  label;
        FUNCTION_TYPE TYPE;
        GUI_TYPE_METADATA typeMetadata;

    };

}

namespace Command::Definition::Input::GUI::Resolver {
    int& resolvePenWidth(Application_State&);
    int& resolveDrawstep(Application_State&);
    int& resolveCanvasHeight(Application_State&);
    int& resolveCanvasWidth(Application_State&);
    colour& resolveDrawColourChange(Application_State&);
    colour& resolveBackgroundColourChange(Application_State&);
}

namespace Command::Definition {

    struct Argument_Metadata {
        std::vector<::Command::Argument::Argument_Definition> arguments;
        std::vector<argument>                               defaultArgs;
    };

    struct Command_Metadata {

        std::string_view                commandDescription;
        Argument_Metadata                 argumentMetadata;
        bool                              undoable = false;

    };

    struct Input_Metadata {

        std::optional<Input::Keyboard_Metadata>    keyboard;
        std::optional<Input::Mouse_Metadata>          mouse;
        std::optional<Input::CLI_Metadata>              cli;
        std::optional<Input::GUI::GUI_Metadata>         gui;

    };

}

namespace Command {

    using Command_Processor = void(*)(Application_State&, Cmd&);

    struct Cmd {

        COMMAND_ID                                        ID;
        std::vector<argument>                           args;
        Command_Processor                          processor;

    };

    struct Command_Definition {

        COMMAND_ID                                   ID;
        Definition::Command_Metadata    commandMetadata;
        Definition::Input_Metadata        inputMetadata;

        Command_Processor                     processor;

    };

}

struct Input_State;

namespace Command::Processor {

    enum class returnCode {
        SUCCESS,
        ARG_COUNT_INVALID,
        ARG_TYPE_INVALID,
        ARG_RANGE_INVALID
    };

    std::pair<int, returnCode> constructCommand(Command_State& s, const Command_Definition* def, std::vector<argument> args = {});
    void processCommands(Application_State&);
}