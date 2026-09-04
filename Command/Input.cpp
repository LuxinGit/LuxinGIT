#include "Input.h"
#include "COMMAND REPO.h"
#include "Application/Application.h"
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

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

		void harvestKeyboardState(Input_State& s) {

			if (ImGui::GetIO().WantCaptureKeyboard) return;

			const bool* kS = SDL_GetKeyboardState(nullptr);
			for (const auto& [scancode, def] : s.KeyboardState.keyBindings) {
				if (kS[scancode] &&
					(def->inputMetadata.keyboard->repeatable || !s.KeyboardState.previousKeyboardState[scancode]))
					Command::Processor::constructCommand(s, def);

				s.KeyboardState.previousKeyboardState[scancode] = kS[scancode];
			}
		}
		void harvestMouseState(Input_State& s, Cursor_State& sC) {

			if (!s.MouseState.enableMouse) return;

			SDL_MouseButtonFlags mS = SDL_GetMouseState(&sC.deltaCursor.first, &sC.deltaCursor.second);

			for (const auto& [button, def] : s.MouseState.mouseBindings) 
				if (mS & button) Command::Processor::constructCommand(s, def);

		}
        void checkShowMouse(Input_State& iS) {
            if (!iS.MouseState.enableMouse or 
                ImGui::GetIO().WantCaptureMouse) SDL_ShowCursor();
            else SDL_HideCursor();
        }
		void harvestUserPeripheralInputs(Application_State& s)
		{
			harvestKeyboardState(s.InputState);
			if (!ImGui::GetIO().WantCaptureMouse)
				harvestMouseState(s.InputState, s.CursorState);
            checkShowMouse(s.InputState);
		}

	}

	void initialiseBindings(Input_State& s)
	{
		for (const auto& def : NEW_COMMAND_REPO) {
			
			if (def.inputMetadata.keyboard) s.KeyboardState.keyBindings		[def.inputMetadata.keyboard->SCANCODE]	= &def;
			if (def.inputMetadata.mouse)	s.MouseState.mouseBindings		[def.inputMetadata.mouse->MOUSECODE]	= &def;
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
                    s.InputState,
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
                    s.InputState,
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
                    s.InputState,
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
                    s.InputState,
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