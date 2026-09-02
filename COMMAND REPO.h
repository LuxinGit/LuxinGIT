#pragma once
#include "Command.h"

using Command_Repo = const std::array<Command_Definition, static_cast<std::size_t>(COMMAND_ID::INVALID)>;
using New_Command_Repo = const std::array<Command::Command_Definition, static_cast<std::size_t>(COMMAND_ID::INVALID)>;
extern Command_Repo COMMAND_REPO;
extern New_Command_Repo NEW_COMMAND_REPO;