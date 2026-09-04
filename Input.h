#pragma once
#include "Command.h"
#include <unordered_set>

struct Application_State;

struct Input_State {

	struct Keyboard_State {
		bool enableKeyboard = ENABLE_KEYBOARD;
		std::unordered_map<SDL_Scancode, const Command::Command_Definition*> keyBindings;
		std::array<bool, SDL_SCANCODE_COUNT> previousKeyboardState{};
	};

	struct Mouse_State {
		bool enableMouse = ENABLE_MOUSE;
		std::unordered_map<SDL_MouseButtonFlags, const Command::Command_Definition*> mouseBindings;
	};

	struct CLI_State {
		bool enableCLI = ENABLE_CLI;
		std::unordered_map<std::string_view, const Command::Command_Definition*> commandLineBindings;
	};

	struct GUI_State {

		bool enableGUI = ENABLE_GUI;

		using def = const Command::Command_Definition*;

		std::array<
			std::vector<def>,
			static_cast<size_t>(Command::Definition::Input::GUI::HEADER::COUNT)
		> headers;

		std::unordered_set<def> openPopouts;
	};

	struct Command_State {

		std::vector<Command::Cmd> commandQueue;

	};

	Keyboard_State	 KeyboardState;
	Mouse_State			MouseState;
	CLI_State			  CLIState;
	GUI_State			  GUIState;
	
	Command_State	  CommandState;

};

namespace Input {

	void initialiseBindings(Input_State& s);

	bool harvestApplicationInputs(Application_State& s);

}

namespace Input::GUI {

	void initialiseGUI(Application_State& s);
	void cleanupGUI();

	void beginFrame();

	void renderGUI(Application_State& s);

}