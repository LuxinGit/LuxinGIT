#pragma once
#include "Command/Command.h"
#include <unordered_map>

namespace Command
{

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
        struct argmd;
        struct dfn;        

        using interpreter   = std::vector<cmd>(*)(const Application_State&, const cmd&);
        using validator     = bool(*)(const Application_State&, const cmd&);
        using processor     = void(*)(Application_State&, cmd&);

        struct cmd
        {
            const dfn* definition;
            std::unordered_map<std::string, ::Command::argument> args;

            bool setArg(const std::string& argName, const ::Command::argument& arg);
            cmd(const dfn* def);
        };

        struct argmd
        {
            ::Command::Argument::ARGTYPE type;
            ::Command::argument defaultValue;
        };

        struct dfn
        {
            std::string name;

            std::unordered_map<std::string, argmd> argDefinitions;
            
            interpreter interp = nullptr;
            validator   valdtr = nullptr;
            processor   prcssr = nullptr;
        };

    }


}