#include "Command.h"
#include "Application/Application.h"

#include <variant>
#include <unordered_map>
#include <array>

namespace Command::Definition::Input::GUI::Resolver {

    int& resolvePenWidth(Application_State& varMH) {
        return varMH.DrawState.pen;
    }
    int& resolveDrawstep(Application_State& varMH) {
        return varMH.CursorState.drawStep;
    }
    int& resolveCanvasWidth(Application_State& varMH)
    {
        return varMH.CanvasState.width;
    }
    int& resolveCanvasHeight(Application_State& varMH)
    {
        return varMH.CanvasState.height;
    }
    colour& resolveDrawColourChange(Application_State& varMH)
    {
        return varMH.DrawState.drawColour;
    }
    colour& resolveBackgroundColourChange(Application_State& varMH)
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

        const Definition::Argument_Metadata& aMD = def->commandMetadata.argumentMetadata;

        Cmd command = { def->ID, {} , def->processor};

        if (aMD.arguments.empty()) {
            if (!args.empty())
                return { 0, returnCode::ARG_COUNT_INVALID };

            addCommand(s, command);
            return { 0, returnCode::SUCCESS };
        }

        if (args.empty()) {

            command.args = aMD.defaultArgs;
            addCommand(s, command);

            return { 0, returnCode::SUCCESS };
        }

        if (args.size() != aMD.arguments.size()) return { 0 , returnCode::ARG_COUNT_INVALID };

        for (int i = 0; i < args.size(); i++) {

            const Argument::Argument_Definition& adef = aMD.arguments[i];
            size_t a = args[i].index();
            if (args[i].index() != static_cast<size_t>(adef.type))
                return { i, returnCode::ARG_TYPE_INVALID };

            if (adef.constraints &&
                !constraintComparison(args[i], *adef.constraints))
                return { i, returnCode::ARG_RANGE_INVALID };

            command.args.emplace_back(args[i]);

        }

        addCommand(s, command);
        return { 0 , returnCode::SUCCESS };

    }

    void processCommands(Application_State& s)
    {
        if (s.InputState.CommandState.commandQueue.size() == 0) Action::commitCurrentAction(s.ActionState);

        for (auto& c : s.InputState.CommandState.commandQueue) {
            c.processor(s, c);
        }

        s.InputState.CommandState.commandQueue = {};

    }

}
