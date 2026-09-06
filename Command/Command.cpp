#include "Command.h"
#include "Application/Application.h"
#include "Input.h"

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

        void addCommand(Command_State& s, Cmd command) {
            s.commandQueue.emplace_back(command);
        }
        void addCommand(Command_State& s, COMMAND_ID ID, std::vector<argument> args, Command_Processor p) {
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



    std::pair<int, returnCode> constructCommand(Command_State& s, const Command_Definition* def, std::vector<argument> args)
    {

        const std::vector<Command::Argument::Argument_Definition>& defArguments = def->commandMetadata.arguments;

        Cmd command = { def->ID, {} , def->processor};

        if (args.size() > defArguments.size()) return { 0 , returnCode::ARG_COUNT_INVALID };
        if (args.size() < defArguments.size()) args.resize(defArguments.size());

        for (int i = 0; i < args.size(); i++) {

            const Argument::Argument_Definition& adef = defArguments[i];
            
            if (std::holds_alternative<std::monostate>(args[i])) {
                if (adef.required)
                    return { static_cast<int>(i), returnCode::ARG_COUNT_INVALID };

                command.args.emplace_back(std::monostate{});
                continue;
            }

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
        if (s.CommandState.commandQueue.size() == 0) Action::commitCurrentAction(s.ActionState);

        for (auto& c : s.CommandState.commandQueue) {
            c.processor(s, c);
        }

        s.CommandState.commandQueue = {};

    }

}
