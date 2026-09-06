#pragma once

#include "Command.h"
#include "Application/CONSTANTS.h"

#include <unordered_set>
#include <unordered_map>

struct Application_State;

struct Input_State {

	struct Keyboard_State {
		bool enableKeyboard = ENABLE_KEYBOARD;
		std::unordered_map<SDL_Scancode, Command::Definition::Input::Binding> keyBindings;
		std::array<bool, SDL_SCANCODE_COUNT> previousKeyboardState{};
	};

	struct Mouse_State {
		bool enableMouse = ENABLE_MOUSE;
		std::unordered_map<SDL_MouseButtonFlags, const Command::Definition::Input::Binding> mouseBindings;
	};

	struct CLI_State {
		bool enableCLI = ENABLE_CLI;
		std::unordered_map<std::string_view, const Command::Command_Definition*> commandLineBindings;
	};

    struct GUI_State {

        using def = const Command::Command_Definition*;

        struct GUI_Binding {

            def definition;
            size_t guiIndex;
            std::vector<Command::argument> args;

            bool operator==(const GUI_Binding& other) const noexcept {
                return
                    definition == other.definition &&
                    guiIndex == other.guiIndex;
            }
        };

        struct GUI_Binding_Hash {

            size_t operator()(const GUI_Binding& b) const noexcept {

                size_t h1 = std::hash<def>{}(b.definition);
                size_t h2 = std::hash<size_t>{}(b.guiIndex);

                return h1 ^ (h2 << 1);
            }
        };

        bool enableGUI = ENABLE_GUI;

        std::array<
            std::vector<GUI_Binding>,
            static_cast<size_t>(
                Command::Definition::Input::GUI::HEADER::COUNT
                )
        > headers;

        std::unordered_set<
            GUI_Binding,
            GUI_Binding_Hash
        > openPopouts;
    };

	Keyboard_State	 KeyboardState;
	Mouse_State			MouseState;
	CLI_State			  CLIState;
	GUI_State			  GUIState;

};

namespace Input {

	void initialiseBindings(Input_State& s);

	bool harvestApplicationInputs(Application_State& s);

	void enableMouse(Application_State& s, Command::Cmd&);

}

namespace Input::GUI {

	void initialiseGUI(Application_State& s);

	void cleanupGUI();

	void beginFrame();

	void renderGUI(Application_State& s);

}

namespace Input::CLI {

	void openCLI(Application_State& s, Command::Cmd&);

}