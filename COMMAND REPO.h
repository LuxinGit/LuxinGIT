#pragma once
#include "Command.h"

using New_Command_Repo = const std::array<Command::Command_Definition, static_cast<std::size_t>(COMMAND_ID::INVALID)>;
extern New_Command_Repo NEW_COMMAND_REPO;