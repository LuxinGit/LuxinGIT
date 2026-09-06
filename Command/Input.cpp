#include "Input.h"
#include "COMMAND REPO.h"
#include "Application/STRUCTS.h"
#include "Application/Application.h"
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <iostream>
#include <cassert>

namespace Input {
	
	namespace {

		bool harvestEventStates()
		{
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				if (ENABLE_GUI) ImGui_ImplSDL3_ProcessEvent(&event);

				if (event.type == SDL_EVENT_QUIT)
					return false;

			}

			return true;

		}

		void harvestKeyboardState(Input_State& s, Command_State& cS) {

			if (ImGui::GetIO().WantCaptureKeyboard) return;

			const bool* kS = SDL_GetKeyboardState(nullptr);
			for (const auto& [scancode, binding] : s.KeyboardState.keyBindings) {

				if (kS[scancode] &&
                    (binding.def.inputMetadata.keyboard.value()[binding.callerIndex].repeatable || !s.KeyboardState.previousKeyboardState[scancode]))
					Command::Processor::constructCommand(cS, &binding.def, binding.args);

				s.KeyboardState.previousKeyboardState[scancode] = kS[scancode];
			}

		}
		void harvestMouseState(Input_State& s, Cursor_State& sC, Command_State& cS) {

			if (!s.MouseState.enableMouse) return;

			SDL_MouseButtonFlags mS = SDL_GetMouseState(&sC.deltaCursor.first, &sC.deltaCursor.second);
            for (const auto& [button, binding] : s.MouseState.mouseBindings)
                if (mS & button)
                    Command::Processor::constructCommand(cS, &binding.def, binding.args);
		}
        void checkShowMouse(Input_State& iS) {
            if (!iS.MouseState.enableMouse or 
                ImGui::GetIO().WantCaptureMouse) SDL_ShowCursor();
            else SDL_HideCursor();
        }
		void harvestUserPeripheralInputs(Application_State& s)
		{
			harvestKeyboardState(s.InputState, s.CommandState);
			if (!ImGui::GetIO().WantCaptureMouse)
				harvestMouseState(s.InputState, s.CursorState, s.CommandState);
            checkShowMouse(s.InputState);
		}

        void initialiseKeyboardBinding(const Command::Command_Definition& def, Input_State& s) {
            
            const auto& kmds = def.inputMetadata.keyboard.value();
            
            for (size_t i = 0; i < kmds.size(); i++) {
                s.KeyboardState.keyBindings.emplace(kmds[i].defaultScancode, Command::Definition::Input::Binding{i, def.commandMetadata.presets[kmds[i].presetIndex].args, def});
            }

        }
        void initialiseMouseBinding(const Command::Command_Definition& def, Input_State& s) {

            const auto& mmds = def.inputMetadata.mouse.value();

            for (size_t i = 0; i < mmds.size(); i++) {
                s.MouseState.mouseBindings.emplace(mmds[i].defaultMousecode, Command::Definition::Input::Binding{ i, def.commandMetadata.presets[mmds[i].presetIndex].args, def });
            }
        }

	}

	void initialiseBindings(Input_State& s)
	{
		for (const auto& def : NEW_COMMAND_REPO) {
			
			if (def.inputMetadata.keyboard) initialiseKeyboardBinding(def, s);
			if (def.inputMetadata.mouse)	initialiseMouseBinding   (def, s);
			if (def.inputMetadata.cli)		s.CLIState.commandLineBindings	[def.inputMetadata.cli->commandName]	= &def;
			if (def.inputMetadata.gui) {
				const Command::Definition::Input::GUI::GUI_Metadata& gui = *def.inputMetadata.gui;
				s.GUIState.headers[static_cast<size_t>(gui.header)].emplace_back(&def);
			}

		}
	}

	bool harvestApplicationInputs(Application_State& s) {

		if (!harvestEventStates()) return false; // false if quit called
			
		harvestUserPeripheralInputs(s);

		return true;

	}

    void enableMouse(Application_State& s, Command::Cmd&) {
        s.InputState.MouseState.enableMouse = !s.InputState.MouseState.enableMouse;
    }

}

namespace Input::GUI {

    namespace {

        using commandDefinition = const Command::Command_Definition*;

        void binaryMenuItem(
            Application_State& s,
            commandDefinition def)
        {
            const Command::Definition::Input::GUI::GUI_Metadata& md =
                *def->inputMetadata.gui;

            if (ImGui::MenuItem(md.label.data())) {
                Command::Processor::constructCommand(
                    s.CommandState,
                    def
                );
            }
        }

        void nonbinaryMenuItem(
            Application_State& s,
            commandDefinition def)
        {
            const Command::Definition::Input::GUI::GUI_Metadata& md =
                *def->inputMetadata.gui;

            auto& openPopouts =
                s.InputState.GUIState.openPopouts;

            if (!ImGui::MenuItem(md.label.data()))
                return;

            if (openPopouts.contains(def))
                openPopouts.erase(def);
            else
                openPopouts.insert(def);
        }

        void menuItem(
            Application_State& s,
            commandDefinition def)
        {
            const Command::Definition::Input::GUI::GUI_Metadata& md =
                *def->inputMetadata.gui;

            switch (md.TYPE) {

            case Command::Definition::Input::GUI::FUNCTION_TYPE::BINARY:
                binaryMenuItem(s, def);
                break;

            default:
                nonbinaryMenuItem(s, def);
                break;
            }
        }

        void menu(
            Application_State& s,
            const char* name,
            const std::vector<commandDefinition>& commands)
        {
            if (!ImGui::BeginMenu(name))
                return;

            for (commandDefinition def : commands)
                menuItem(s, def);

            ImGui::EndMenu();
        }

        void sliderPopout(
            Application_State& s,
            commandDefinition def)
        {
            const Command::Definition::Input::GUI::GUI_Metadata& md =
                *def->inputMetadata.gui;

            const Command::Definition::Input::GUI::SLIDER_METADATA& sMD =
                std::get<Command::Definition::Input::GUI::SLIDER_METADATA>(
                    md.typeMetadata
                );

            int& underlying = sMD.resolve(s);
            int payload = underlying;

            if (ImGui::SliderInt(
                md.label.data(),
                &payload,
                sMD.minimum,
                sMD.maximum))
            {
                Command::Processor::constructCommand(
                    s.CommandState,
                    def,
                    { payload }
                );
            }
        }

        void colourPopout(
            Application_State& s,
            commandDefinition def)
        {
            const Command::Definition::Input::GUI::GUI_Metadata& md =
                *def->inputMetadata.gui;

            const Command::Definition::Input::GUI::COLOUR_METADATA& cMD =
                std::get<Command::Definition::Input::GUI::COLOUR_METADATA>(
                    md.typeMetadata
                );

            colour& underlying = cMD.resolve(s);

            std::array<float, 4> displayload =
                luxel::coloursToFloat(underlying);

            if (ImGui::ColorEdit4(
                md.label.data(),
                displayload.data()))
            {
                Command::Processor::constructCommand(
                    s.CommandState,
                    def,
                    {
                        luxel::floatsToColour(displayload)
                    }
                );
            }

            int rgba[4] = {
                static_cast<int>(underlying[0]),
                static_cast<int>(underlying[1]),
                static_cast<int>(underlying[2]),
                static_cast<int>(underlying[3])
            };

            if (ImGui::InputInt4("RGBA", rgba)) {

                colour value = {};

                for (size_t i = 0; i < value.size(); i++) {
                    value[i] = static_cast<uint8_t>(
                        std::clamp(rgba[i], 0, 255)
                        );
                }

                Command::Processor::constructCommand(
                    s.CommandState,
                    def,
                    { value }
                );
            }
        }

        bool renderPopout(
            Application_State& s,
            commandDefinition def)
        {
            const Command::Definition::Input::GUI::GUI_Metadata& md =
                *def->inputMetadata.gui;

            bool open = true;

            if (!ImGui::Begin(
                md.label.data(),
                &open))
            {
                ImGui::End();
                return open;
            }

            switch (md.TYPE) {

            case Command::Definition::Input::GUI::FUNCTION_TYPE::SLIDER:
                sliderPopout(s, def);
                break;

            case Command::Definition::Input::GUI::FUNCTION_TYPE::COLOUR:
                colourPopout(s, def);
                break;

            default:
                break;
            }

            ImGui::End();

            return open;
        }

        void checkForPopouts(Application_State& s)
        {
            auto& openPopouts =
                s.InputState.GUIState.openPopouts;

            std::vector<commandDefinition> clearList;

            for (commandDefinition def : openPopouts) {
                if (!renderPopout(s, def))
                    clearList.emplace_back(def);
            }

            for (commandDefinition def : clearList)
                openPopouts.erase(def);
        }

        void renderMenuBar(Application_State& s)
        {
            auto& headers =
                s.InputState.GUIState.headers;

            if (!ImGui::BeginMainMenuBar())
                return;

            menu(
                s,
                "File",
                headers[
                    static_cast<size_t>(Command::Definition::Input::GUI::HEADER::FILE)
                ]
            );

            menu(
                s,
                "Edit",
                headers[
                    static_cast<size_t>(Command::Definition::Input::GUI::HEADER::EDIT)
                ]
            );

            menu(
                s,
                "Tools",
                headers[
                    static_cast<size_t>(Command::Definition::Input::GUI::HEADER::TOOLS)
                ]
            );

            ImGui::EndMainMenuBar();
        }

    }

    void initialiseGUI(Application_State& s)
    {
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplSDL3_InitForSDLRenderer(
            s.SDLState.Window,
            s.SDLState.Renderer
        );

        ImGui_ImplSDLRenderer3_Init(
            s.SDLState.Renderer
        );

        ImGuiIO& io = ImGui::GetIO();

        io.ConfigFlags |=
            ImGuiConfigFlags_NoMouseCursorChange;
    }

    void beginFrame()
    {
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }

    void cleanupGUI()
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }


    void renderGUI(Application_State& s)
    {
        if (!s.InputState.GUIState.enableGUI)
            return;

        renderMenuBar(s);
        checkForPopouts(s);

        ImGui::Render();

        ImGui_ImplSDLRenderer3_RenderDrawData(
            ImGui::GetDrawData(),
            s.SDLState.Renderer
        );
    }

}

namespace Input::CLI {

    namespace {
    
        std::string harvestInput(const std::string& Question, bool linebreak = true) 
        {
            std::cout << Question << std::endl;
            std::string output;
            std::cin >> output;
            if (linebreak) std::cout << std::endl;
            return output;
        }

        bool safestoi(const std::string& s, int& i) {
            
            try {
                i = std::stoi(s);
            } 
            catch (...) {
                return false;
            }
            return true;

        }

        bool harvestIntegerInput(const std::string& q, int& i) {

            while (!safestoi(harvestInput(q), i)) {
                std::cout << "Invalid input (not a number?)" << std::endl;
                if (harvestInput("Quit input? [Y]") == "Y") return false;
            }

            return true;

        }

        bool harvestArgumentInput(std::vector<int>& ret, const std::string& tname) {
            
            for (size_t i = 0; i < ret.size(); i++) {
                if (!harvestIntegerInput("[" + tname + "] Enter value " + std::to_string(i + 1) + ":", ret[i]))
                    return false;
            }
            return true;

        }

        colour convertVecToColour(const std::vector<int>& v) {
            colour ret = {};
            
            for (size_t i = 0; i < v.size(); i++) {
                ret[i] = static_cast<uint8_t>(v[i]);
            }

            return ret;

        }

        bool harvestArgument(Command::Argument::ARGTYPE t, Command::argument& arg) 
        {
            std::vector<int> ret;
            switch (t) {
            case Command::Argument::ARGTYPE::INT:

                ret = std::vector<int>(1);
                if (!harvestArgumentInput(ret, "INT")) return false;
                arg = ret[0];
                return true;

            case Command::Argument::ARGTYPE::COORDINATE:
                
                ret = std::vector<int>(2);
                if (!harvestArgumentInput(ret, "COORDINATE")) return false;
                arg = coordinate{ ret[0], ret[1] };
                return true;

            case Command::Argument::ARGTYPE::COLOUR:
                
                ret = std::vector<int>(4);
                if (!harvestArgumentInput(ret, "COLOUR")) return false;
                arg = convertVecToColour(ret);
                return true;

            default:
                return false;
            }
        }

        bool createCommand(Input_State::CLI_State& CLIS, Command_State& cS, const std::string& input) {

            if (!CLIS.commandLineBindings.contains(input)) return false;

            auto def = CLIS.commandLineBindings.at(input);

            const auto& defArgs = def->commandMetadata.arguments;
                // std::vector<argument_definition>

            if (defArgs.empty()
                or harvestInput("Use default arguments? [Y]") == "Y") {
                Command::Processor::constructCommand(cS, def);
                return true;
            }

            std::vector<Command::argument> args = std::vector<Command::argument>(defArgs.size());

            for (size_t i = 0; i < defArgs.size(); i++) {

                if (!defArgs[i].required)
                    if (harvestInput("This argument (" + std::string(defArgs[i].name) + ") is not required.  Input anyway? [Y]") != "Y")
                        continue;
                std::cout << "Entering argument " << std::string(defArgs[i].name) << std::endl;
                if (!harvestArgument(defArgs[i].type, args[i]))
                    return false;

            }
            
            while (true) {

                // Yeah like if you're really bored you can refactor this.

                const auto& retVal = Command::Processor::constructCommand(cS, def, args);

                switch (retVal.second) {
                case Command::Processor::returnCode::ARG_RANGE_INVALID:
                    if (harvestInput("Argument[" + std::to_string(retVal.first) + "] not within range.  Replace?\n [Y] (or quits otherwise)") != "Y")
                        return false;
                    if (defArgs[retVal.first].type == Command::Argument::ARGTYPE::INT) {
                        std::cout << "Expected range:\nMin: " + std::to_string(std::get<int>(def->commandMetadata.arguments[retVal.first].constraints->first));
                        std::cout << "\nMax: " + std::to_string(std::get<int>(def->commandMetadata.arguments[retVal.first].constraints->second));
                    }
                    else {
                        std::cout << "Expected range:\nMin: " + static_cast<std::string>(std::get<coordinate>(def->commandMetadata.arguments[retVal.first].constraints->first));
                        std::cout << "\nMax: " + static_cast<std::string>(std::get<coordinate>(def->commandMetadata.arguments[retVal.first].constraints->second));
                    }
                    std::cout << std::endl;
                    harvestArgument(defArgs[retVal.first].type, args[retVal.first]);
                    break;
                default:
                    return true;
                    // we could sanity check other stuff here but given that the other edge cases are handled within harvest argument I'm not too concerned.
                    // worth noting that colour currently doesn't have a constraint check but I can't think of a valid use case where this would be a concern.
                }
            }

        }

        bool retrieveHelp(Input_State::CLI_State& CLIS, const std::string& input) 
        {
            
            if (!CLIS.commandLineBindings.contains(input)) return false;
            auto def = CLIS.commandLineBindings.at(input);
            std::cout << def->inputMetadata.cli->help << std::endl;
            return true;

        }

		void CLILoop(Application_State& s) {
            Input_State::CLI_State& CLIS = s.InputState.CLIState;
            Command_State& cS = s.CommandState;
			while (true) {
				std::cout << R"(
You are at the beginning of the CLI loop.

Options:

    [1] Add command.
    [2] Execute current commands.
    [3] Get help for a command name.
    [4] List all command names.
    [5] Quit.

)" << std::endl;
				switch (std::stoi(harvestInput(""))) {
				case 1:
                    if (createCommand(CLIS, cS, harvestInput("Enter the name of the command: ")) == false)
                        std::cout << "No command generated." << std::endl;
                    else 
                        std::cout << "Command added successfully!" << std::endl;
					break;
				case 2:
                    Command::Processor::processCommands(s);
					break;
                case 3:
                    if (!retrieveHelp(CLIS, harvestInput("Enter the name of the command you would like more information on:")))
                        std::cout << "Invalid command name." << std::endl;
                    break;
                case 4:
                    for (const auto& [binding, def] : s.InputState.CLIState.commandLineBindings) {
                        std::cout << def->inputMetadata.cli->commandName << std::endl;
                    }
                    break;
				case 5:
					if (cS.commandQueue.size()) if (harvestInput("You have unexecuted commands.  Continue? [Y]") != "Y") break;
                    cS.commandQueue = {};
					return;
				default:
					continue;
				}
			}

		}

	}

    void openCLI(Application_State& s, Command::Cmd&)
    {
        s.CommandState.commandQueue = {};
        CLILoop(s);
    }

}