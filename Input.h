#pragma once
#include "COMMAND.h"
#include <unordered_set>

struct Input_State {

	bool enableCLI			= ENABLE_CLI;
	bool enableMouse		= ENABLE_MOUSE;
	bool enableKeyboard		= ENABLE_KEYBOARD;
	bool enableGUI			= ENABLE_GUI;

	std::unordered_map<SDL_Scancode,			const Command_Definition*> keyBindings;
	std::unordered_map<SDL_MouseButtonFlags,	const Command_Definition*> mouseBindings;
	std::unordered_map<std::string,				const Command_Definition*> commandLineBindings;
	
	std::unordered_map<COMMAND_ID, GUI_METADATA>							labelBindings{};
	std::array<std::vector<COMMAND_ID>, static_cast<int>(GUI_METADATA::HEADER::COUNT)> headers;
	std::unordered_set<COMMAND_ID> openPopouts = {};

	std::array<bool, SDL_SCANCODE_COUNT> previousKeyboardState{};

};

namespace Input {



}