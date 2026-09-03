#pragma once

#include <variant>
#include <unordered_map>
#include <array>
#include <optional>
#include <string>

#include <SDL3/SDL.h>

#include "STRUCTS.h"

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

//struct Command_OLD {
//
//public:
//
//    enum class TYPE {
//        DRAW = 0,
//        MOVE = 1,
//        META = 2,
//        APP = 3
//    };
//        struct MOVE {
//
//            enum class ACTION {
//                DIRECTION = 0,
//                SET = 1
//            };
//
//            enum class DIRECTION {
//                RIGHT = 0,
//                DOWN = 1,
//                LEFT = 2,
//                UP = 3
//            };
//            enum class SET {
//                RESET_TO_ORIGIN = 0,
//                USE_PAYLOAD = 1
//            };
//
//        private:
//
//            MOVE(ACTION varAction, int varSetting = 0);
//
//        public:
//
//            ACTION action;
//            int setting;
//
//            MOVE(DIRECTION varSetting);
//            MOVE(SET varSetting);
//
//        };
//        struct DRAW {
//
//            enum class ACTION {
//                LINE = 0,
//                CIRCLE = 1,
//                POINT = 2,
//                FILL = 10
//            };
//
//            enum class LINE {
//                NORMAL = 0
//            };
//            enum class CIRCLE {
//                NORMAL = 0,
//                RAINBOW = 1
//            };
//            enum class POINT {
//                USE_PAYLOAD = 0,
//                USE_CURSOR = 1
//            };
//
//            enum class FILL {
//                USE_PAYLOAD = 0,
//                USE_DRAW_COLOUR = 1
//            };
//
//        private:
//
//            DRAW(ACTION varAction, int varSetting = 0);
//
//        public:
//
//            ACTION action;
//            int setting;
//
//            DRAW(LINE varSetting);
//            DRAW(CIRCLE varSetting);
//            DRAW(POINT varSetting);
//            DRAW(FILL varSetting);
//        };
//        struct META {
//
//            enum class ACTION {
//                RESET,
//                CHANGE_COLOUR,
//                CHANGE_DRAWSTEP,
//                CHANGE_PENMODE,
//                CHANGE_PEN_WIDTH,
//                SAVE_ORIGIN,
//                CANVAS_CHANGE_SIZE
//            };
//
//            enum class RESET {
//                RESET_CANVAS = 0,
//                RESET_CURSOR = 1,
//                RESET_ACTION_QUEUE = 2,
//                RESET_ALL = 3,
//            };
//            enum class CHANGE_COLOUR {
//                DEFAULT = 0,
//                USE_PAYLOAD_DRAW = 1,
//                USE_PAYLOAD_BACKGROUND = 2,
//                RANDOM = 3,
//                USE_HOVERED_LUXEL = 4
//            };
//            enum class CHANGE_DRAWSTEP {
//                ADD_PAYLOAD = 0,
//                SET_TO_PAYLOAD = 1
//            };
//            enum class CHANGE_PENMODE {
//                DRAW = 0,
//                PEN_DOWN = 1,
//                RUBBER = 2,
//                RAINBOW = 3
//            };
//            enum class CHANGE_PEN_WIDTH {
//                ADD_PAYLOAD = 0,
//                SET_TO_PAYLOAD = 1
//            };
//            enum class SAVE_ORIGIN {
//                NORMAL = 0
//            };
//
//            enum class PENMODE_PDOWN_PINTERP {
//                DISCRETE = 0,
//                CONTINUOUS = 1
//            };
//
//            enum class CANVAS_CHANGE_SIZE {
//                ADD_PAYLOAD = 0,
//                SET_TO_PAYLOAD = 1,
//                SET_WIDTH_TO_PAYLOAD = 2,
//                SET_HEIGHT_TO_PAYLOAD = 3
//            };
//
//        private:
//
//            META(ACTION varAction, int varSetting);
//
//        public:
//
//            ACTION action;
//            int setting;
//
//            META(RESET varSetting);
//            META(CHANGE_COLOUR varSetting);
//            META(CHANGE_DRAWSTEP varSetting);
//            META(CHANGE_PENMODE varSetting);
//            META(CHANGE_PEN_WIDTH varSetting);
//            META(SAVE_ORIGIN varSetting);
//            META(CANVAS_CHANGE_SIZE varSetting);
//
//        };
//        struct APP {
//
//            enum class ACTION {
//                INPUT_MODE = 0,
//                UNDO_REDO
//            };
//
//            enum class INPUT_MODE {
//                CLI = 0,
//                MOUSE = 1
//            };
//
//            enum class UNDO_REDO {
//                UNDO = 0,
//                REDO = 1
//            };
//
//        private:
//
//            APP(ACTION varAction, int varSetting = 0);
//
//        public:
//
//            APP(INPUT_MODE varSetting);
//            APP(UNDO_REDO varSetting);
//
//            ACTION action;
//            int setting;
//
//        };
//
//    using Payload_OLD = std::variant<std::monostate, int, std::pair<float, float>, colour, coordinate>;
//
//private:
//
//    Command_OLD(COMMAND_ID varID, TYPE vartype, int varaction, int varsetting, bool varRepeatable, Payload_OLD varPayload = {});
//
//public:
//
//    Command_OLD(COMMAND_ID varID, MOVE cmd, bool repeatable, Payload_OLD payload = {});
//    Command_OLD(COMMAND_ID varID, DRAW cmd, bool repeatable, Payload_OLD payload = {});
//    Command_OLD(COMMAND_ID varID, META cmd, bool repeatable, Payload_OLD payload = {});
//    Command_OLD(COMMAND_ID varID, APP cmd, bool repeatable, Payload_OLD payload = {});
//
//    TYPE type;
//    int action; // differentiates between commands.
//    int setting; // settings within commands.
//    bool repeatable;
//    Payload_OLD payload;
//
//    COMMAND_ID ID;
//
//};
//
//enum class COMMAND_PROCESSOR_ID {
//    CANVAS_HANDLER,
//    CURSOR_HANDLER,
//    DRAW_HANDLER,
//    ACTION_HANDLER,
//    CLI_HANDLER,
//    KEYBOARD_HANDLER,
//    MOUSE_HANDLER
//};
//
//struct GUI_METADATA {
//    enum class HEADER {
//        FILE = 0,
//        EDIT = 1,
//        TOOLS = 2,
//        COUNT
//    };
//
//    enum class FUNCTION_TYPE {
//        BINARY,
//        SLIDER,
//        COLOUR
//    };
//
//    struct SLIDER_METADATA {
//        int minimum;
//        int maximum;
//        using Resolver = int* (*)(Master_Handler&);
//        Resolver getUnderlying;
//        int* underlying = nullptr; // used as reference for value, such that slider knows where we're at with whatever we're talking about.
//        static int* resolvePenWidth(Master_Handler&);
//        static int* resolveDrawstep(Master_Handler&);
//        static int* resolveCanvasHeight(Master_Handler&);
//        static int* resolveCanvasWidth(Master_Handler&);
//
//    };
//
//    struct COLOUR_METADATA {
//        using Resolver = colour* (*)(Master_Handler&);
//        Resolver getUnderlying;
//        colour* underlying = nullptr;
//        static colour* resolveDrawColourChange(Master_Handler& varMH);
//        static colour* resolveBackgroundColourChange(Master_Handler& varMH);
//    };
//
//    HEADER header;
//    FUNCTION_TYPE functionType;
//    std::string_view label;
//    std::variant<std::monostate, SLIDER_METADATA, COLOUR_METADATA> metadata;
//
//    GUI_METADATA(HEADER varH, std::string_view varL) :
//        header(varH), functionType(FUNCTION_TYPE::BINARY), label(varL) {}
//    GUI_METADATA(HEADER varH, std::string_view varL, SLIDER_METADATA varMD)
//        : header(varH), functionType(FUNCTION_TYPE::SLIDER), label(varL), metadata(varMD) {}
//    GUI_METADATA(HEADER varH, std::string_view varL, COLOUR_METADATA varMD)
//        : header(varH), functionType(FUNCTION_TYPE::COLOUR), label(varL), metadata(varMD) {}
//};
//
//using OLD_Command_Processor = void(*)(Master_Handler&, Command_OLD&);
//
//struct Command_Definition {
//
//    Command_OLD command;
//    COMMAND_PROCESSOR_ID processor;
//
//    std::optional<SDL_Scancode> keyBinding;
//    std::optional<SDL_MouseButtonFlags> mouseBinding;
//    std::optional<std::string_view> cliBinding;
//    std::optional<GUI_METADATA> guiBinding;
//
//    OLD_Command_Processor newProcessor = nullptr;
//
//};
//
//inline std::unordered_map<COMMAND_ID, const Command_Definition*> COMMAND_ID_DEF_MAP = {};
/* TO ADD COMMANDS
* 
*   1. Create COMMAND_ID. This should describe what the action does.
*   2. As necessary, create Command logic (e.g Command::Draw::Circle)
*   3. Add to COMMAND_REPO. Ensure COMMAND_PROCESSOR_ID matches correct processor.
* 
*   BELOW ARE SOME TEMPLATES FOR MAKING NEW processCommands.  
* 
* 
* void process__C__Command(const Command& command) {
    
    using T = Command::TYPE;
    
    switch (command.type) {
    case T::A:
        process__A__Command(command);
        break;
    }
    
}
void process__T__Command(const Command& command) {

    using A = Command::T::ACTION;

    switch (static_cast<A>(command.action)) {
    case (A::INPUT_MODE):
            process__A__Command(command);
            break;
        }
    }
void process__A__Command(const Command& command) {

    using S = Command::T::A

    switch (static_cast<S>(command.setting)) {
    case (S::SETTING_MODE):
        process__S__Command(command);
        break;
    }
}
* 
*   
* 
* 
* 
*/

//// NEW

namespace Command {

    using argument = std::variant<
        int, 
        colour, 
        coordinate>;

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
    };

    struct Argument_Metadata {
        std::vector<Argument_Definition> arguments;
        std::vector<argument> defaultArgs;
    };

    struct Command_Metadata {

        std::string_view                commandDescription;
        Argument_Metadata               argumentMetadata;

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