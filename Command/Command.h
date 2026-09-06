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
    
    CURSOR_MOVE,
    CURSOR_ORIGIN,
    CURSOR_DRAWSTEP,
    DRAW_CIRCLE,
    DRAW_FILL,
    COLOUR,
    PENMODE,
    PEN_DOWN,
    PEN_WIDTH,
    INPUT_CLI_ENABLE,
    INPUT_MOUSE_ENABLE,

    CANVAS_RESIZE_SET_PAYLOAD,
    CANVAS_RESIZE_SET_WIDTH,
    CANVAS_RESIZE_SET_HEIGHT,
    
    UNDO,
    REDO,

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

    struct Argument_Preset {
        std::string_view        name;
        std::vector<argument>   args;
    };

}

namespace Command::Definition::Input {

    struct Binding {
        
        std::size_t callerIndex;
        std::vector<argument> args;
        const Command_Definition& def;

    };

    struct Keyboard_Metadata {

        SDL_Scancode   defaultScancode;
        std::size_t    presetIndex;
        bool           repeatable = false;

    };

    struct CLI_Metadata {

        std::string_view    commandName;
        std::string_view    help;
    };

    struct Mouse_Metadata {

        SDL_MouseButtonFlags    defaultMousecode;
        std::size_t             presetIndex;

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

    struct Command_Metadata {

        std::string_view                             commandDescription;
        std::vector<::Command::Argument::Argument_Definition> arguments;
        std::vector<Argument::Argument_Preset>              presets ={};
        bool                                           undoable = false;

    };

    struct Input_Metadata {

        std::optional<std::vector<Input::Keyboard_Metadata>>    keyboard;
        std::optional<std::vector<Input::Mouse_Metadata>>          mouse;
        std::optional<Input::CLI_Metadata>                           cli;
        std::optional<Input::GUI::GUI_Metadata>                      gui;

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