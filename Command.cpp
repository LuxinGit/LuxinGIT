#include "COMMAND.h"
#include "Master Handler.h"

#include <variant>
#include <unordered_map>
#include <array>

Command_OLD::Command_OLD(COMMAND_ID varID, Command_OLD::TYPE vartype, int varaction, int varsetting, bool varRepeatable, Command_OLD::Payload_OLD varPayload) :
    type(vartype), action(varaction), setting(varsetting), repeatable(varRepeatable), ID(varID), payload(varPayload) {} //cpp
    Command_OLD::Command_OLD(COMMAND_ID varID, MOVE cmd, bool repeatable, Payload_OLD payload)
        : Command_OLD(
            varID,
            TYPE::MOVE,
            static_cast<int>(cmd.action),
            static_cast<int>(cmd.setting),
            repeatable,
            payload
        )
    {}
    Command_OLD::Command_OLD(COMMAND_ID varID, DRAW cmd, bool repeatable, Payload_OLD payload)
        : Command_OLD(
            varID,
            TYPE::DRAW,
            static_cast<int>(cmd.action),
            static_cast<int>(cmd.setting),
            repeatable,
            payload
        )
    {}
    Command_OLD::Command_OLD(COMMAND_ID varID, META cmd, bool repeatable, Payload_OLD payload)
        : Command_OLD(
            varID,
            TYPE::META,
            static_cast<int>(cmd.action),
            static_cast<int>(cmd.setting),
            repeatable,
            payload
        )
    {}
    Command_OLD::Command_OLD(COMMAND_ID varID, APP cmd, bool repeatable, Payload_OLD payload)
        : Command_OLD(
            varID,
            TYPE::APP,
            static_cast<int>(cmd.action),
            static_cast<int>(cmd.setting),
            repeatable,
            payload
        )
    {}


Command_OLD::MOVE::MOVE(ACTION varAction, int varSetting) : action(varAction), setting(varSetting) {}
    Command_OLD::MOVE::MOVE(DIRECTION varSetting)
        : MOVE(ACTION::DIRECTION, static_cast<int>(varSetting)) {}
    Command_OLD::MOVE::MOVE(SET varSetting)
        : MOVE(ACTION::SET, static_cast<int>(varSetting)) {}


Command_OLD::DRAW::DRAW(ACTION varAction, int varSetting)
    : action(varAction), setting(varSetting) {}
    Command_OLD::DRAW::DRAW(LINE varSetting)
        : DRAW(ACTION::LINE, static_cast<int>(varSetting)) {}
    Command_OLD::DRAW::DRAW(CIRCLE varSetting)
        : DRAW(ACTION::CIRCLE, static_cast<int>(varSetting)) {}
    Command_OLD::DRAW::DRAW(POINT varSetting)
        : DRAW(ACTION::POINT, static_cast<int>(varSetting)) {}
    Command_OLD::DRAW::DRAW(FILL varSetting)
        : DRAW(ACTION::FILL, static_cast<int>(varSetting)) {}


Command_OLD::META::META(ACTION varAction, int varSetting)
    : action(varAction), setting(varSetting) {}
    Command_OLD::META::META(RESET varSetting)
        : META(ACTION::RESET, static_cast<int>(varSetting)) {}
    Command_OLD::META::META(CHANGE_COLOUR varSetting)
        : META(ACTION::CHANGE_COLOUR, static_cast<int>(varSetting)) {}
    Command_OLD::META::META(CHANGE_DRAWSTEP varSetting)
        : META(ACTION::CHANGE_DRAWSTEP, static_cast<int>(varSetting)) {}
    Command_OLD::META::META(CHANGE_PENMODE varSetting)
        : META(ACTION::CHANGE_PENMODE, static_cast<int>(varSetting)) {}
    Command_OLD::META::META(CHANGE_PEN_WIDTH varSetting)
        : META(ACTION::CHANGE_PEN_WIDTH, static_cast<int>(varSetting)) {}
    Command_OLD::META::META(SAVE_ORIGIN varSetting)
        : META(ACTION::SAVE_ORIGIN, static_cast<int>(varSetting)) {}
    Command_OLD::META::META(CANVAS_CHANGE_SIZE varSetting)
        : META(ACTION::CANVAS_CHANGE_SIZE, static_cast<int>(varSetting)) {}


Command_OLD::APP::APP(ACTION varAction, int varSetting) : action(varAction), setting(varSetting) {}
    Command_OLD::APP::APP(INPUT_MODE varSetting) : APP(ACTION::INPUT_MODE, static_cast<int>(varSetting)) {}
    Command_OLD::APP::APP(UNDO_REDO varSetting) : APP(ACTION::UNDO_REDO, static_cast<int>(varSetting)) {}

int* GUI_METADATA::SLIDER_METADATA::resolvePenWidth(Master_Handler& varMH) {
    return &varMH.DrawState.pen;
}
int* GUI_METADATA::SLIDER_METADATA::resolveDrawstep(Master_Handler& varMH) {
    return &varMH.CursorState.drawStep;
}
int* GUI_METADATA::SLIDER_METADATA::resolveCanvasWidth(Master_Handler& varMH)
{
    return &varMH.CanvasState.width;
}
int* GUI_METADATA::SLIDER_METADATA::resolveCanvasHeight(Master_Handler& varMH)
{
    return &varMH.CanvasState.height;
}

colour* GUI_METADATA::COLOUR_METADATA::resolveDrawColourChange(Master_Handler& varMH) {
    return &varMH.DrawState.drawColour;
}
colour* GUI_METADATA::COLOUR_METADATA::resolveBackgroundColourChange(Master_Handler& varMH) {
    return &varMH.DrawState.backgroundColour;
}

namespace Command::GUIResolver {

    int& resolvePenWidth(Master_Handler& varMH) {
        return varMH.DrawState.pen;
    }
    int& resolveDrawstep(Master_Handler& varMH) {
        return varMH.CursorState.drawStep;
    }
    int& resolveCanvasWidth(Master_Handler& varMH)
    {
        return varMH.CanvasState.width;
    }
    int& resolveCanvasHeight(Master_Handler& varMH)
    {
        return varMH.CanvasState.height;
    }
    colour& resolveDrawColourChange(Master_Handler& varMH)
    {
        return varMH.DrawState.drawColour;
    }
    colour& resolveBackgroundColourChange(Master_Handler& varMH)
    {
        return varMH.DrawState.backgroundColour;
    }

}

namespace Command::Processor {

    namespace {

        void addCommand(Input_State& s, Cmd command) {
            s.CommandState.commandQueue.emplace_back(command);
        }
        void addCommand(Input_State& s, COMMAND_ID ID, std::vector<argument> args, Command_Processor p) {
            addCommand(s, { ID, args, p});
        }

        template<typename T>
        bool withinConstraints(
            const T& value,
            const T& minimum,
            const T& maximum)
        {
            return value >= minimum && value <= maximum;
        }
        bool withinConstraints(
            const colour& value,
            const colour& minimum,
            const colour& maximum)
        {
            for (size_t i = 0; i < value.size(); i++) {
                if (value[i] < minimum[i] || value[i] > maximum[i])
                    return false;
            }

            return true;
        }
        bool constraintComparison(
            const argument& arg,
            const std::pair<argument, argument>& constraints)
        {
            return std::visit(
                [&](const auto& value) -> bool {
                    using T = std::decay_t<decltype(value)>;

                    if constexpr (std::is_same_v<T, std::monostate>) {
                        return false;
                    }
                    else {
                        return withinConstraints(
                            value,
                            std::get<T>(constraints.first),
                            std::get<T>(constraints.second)
                        );
                    }
                },
                arg
            );
        }

    }



    std::pair<int, returnCode> constructCommand(Input_State& s, const Command_Definition* def, std::vector<argument> args)
    {
        const Argument_Metadata& aMD = def->commandMetadata.argumentMetadata;

        Cmd command = { def->ID, {} , def->processor};

        if (aMD.arguments.empty()) {
            if (!args.empty())
                return { 0, returnCode::ARG_COUNT_INVALID };

            addCommand(s, command);
            return { 0, returnCode::SUCCESS };
        }

        if (args.empty()) {

            addCommand(s, command);
            return { 0, returnCode::SUCCESS };
        }

        if (args.size() != aMD.arguments.size()) return { 0 , returnCode::ARG_COUNT_INVALID };

        for (int i = 0; i < args.size(); i++) {

            const Argument_Definition& adef = aMD.arguments[i];

            if (args[i].index() != static_cast<int>(adef.type))
                return { i, returnCode::ARG_TYPE_INVALID };

            if (adef.constraints &&
                !constraintComparison(args[i], *adef.constraints))
                return { i, returnCode::ARG_RANGE_INVALID };

            command.args.emplace_back(args[i]);

        }

        addCommand(s, command);
        return { 0 , returnCode::SUCCESS };

    }

    void processCommands(Master_Handler& s)
    {
        for (auto& c : s.InputState.CommandState.commandQueue) {
            c.processor(s, c);
        }
    }

}
