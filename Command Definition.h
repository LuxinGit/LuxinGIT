#pragma once
#include "Command/Command.h"
#include <unordered_map>

namespace Command
{
    struct Cmd;

    namespace Definition
    {
        struct Dfn;
        struct Transition;


        // =====================================================================
        // PROCESSING
        // =====================================================================

        // Interpreter:
        // - reads application state
        // - transforms arguments while lowering
        // - cannot mutate application state
        // - does not choose the target definition

        using Interpreter = void(*)(const Application_State&, std::vector<Command::argument>&);


        // Processor:
        // - exists only on primitive definitions
        // - performs the actual state mutation
        // - generates / records the primitive inverse

        using Processor = void(*)(Application_State&, Command::Cmd&);


        // =====================================================================
        // COMMAND GRAPH
        // =====================================================================

        struct Transition
        {
            Dfn* target = nullptr;
            Interpreter interpreter = nullptr;
        };


        struct Dfn
        {
            std::string name;

            // Canonical explicitly-named routes.
            std::unordered_map<std::string, Transition> transitions = {};

            // Shorthand routes into the canonical transitions above.
            std::unordered_map<Command::Argument::ARGTYPE, Transition*> implicitTransitions = {};

            // Non-null only for primitive state-mutating leaves.
            Processor processor = nullptr;
        };
    }


    // =========================================================================
    // RUNTIME COMMAND
    // =========================================================================

    struct Cmd
    {
        Definition::Dfn* definition = nullptr;
        std::vector<argument> args = {};
    };
}