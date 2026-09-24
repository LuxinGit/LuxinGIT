#pragma once
#include "Command/Command.h"
#include <unordered_map>

namespace Command
{
    struct Cmmd;

    namespace Definition
    {
        struct Dfn;
        struct Transition;

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

        using Processor = void(*)(Application_State&, Command::Cmmd&);

        struct Transition
        {
            Dfn* target = nullptr;
            Interpreter interpreter = nullptr;
        };


        struct Dfn
        {
            std::string name;

            // Explicit routes
            std::unordered_map<std::string, Transition> transitions = {};

            // Implicit
            std::unordered_map<Command::Argument::ARGTYPE, Transition*> implicitTransitions = {};

            Processor processor = nullptr;
        };
    }


    // =========================================================================
    // RUNTIME COMMAND
    // =========================================================================

    //struct 
    //{
    //    Definition::Dfn* definition = nullptr;
    //    std::vector<argument> args = {};
    //};asdadssdfsdfasdaasdasdadsjkdflkgjdflkasd


    namespace random
    {
        struct cmd;

        using Processor = void(*)(Application_State&, cmd&);

        struct cdef
        {
            std::string name;
            Processor processor = nullptr;
        };

        struct cmd
        {
            cdef* def = nullptr;
            std::unordered_map<std::string, ::Command::argument> args;
        };

        template<typename T>
        T& arg(cmd& command, const std::string& name)
        {
            return std::get<T>(command.args.at(name));
        }

    }


}