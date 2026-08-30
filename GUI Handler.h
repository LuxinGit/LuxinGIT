#pragma once
#include "COMMAND.h"
#include <imgui.h>
#include <string>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include "SDL Handler.h"
#include <unordered_set>

struct Master_Handler;

struct GUI_Handler {

	inline static SDL_Window*&		Window		= SDL_Handler::Window;
	inline static SDL_Renderer*&	Renderer	= SDL_Handler::Renderer;

	std::array<std::vector<COMMAND_ID>, static_cast<int>(GUI_METADATA::HEADER::COUNT)> headers;
	std::unordered_map<COMMAND_ID, GUI_METADATA> labelMapping;
	std::unordered_set<COMMAND_ID> openPopouts = {};

	Master_Handler& MasterHandler;

	GUI_Handler(Master_Handler&);

	void initialise();
		void initialiseBinding(const Command_Definition&);

	void beginFrame();

	void menu(const std::string& name, const std::vector<COMMAND_ID>& commands);
		void menuItem(const COMMAND_ID&);
			void binaryMenuItem(const COMMAND_ID&, GUI_METADATA&);
			void sliderMenuItem(const COMMAND_ID&, GUI_METADATA&);

	void checkForPopouts();
		bool renderPopout(const COMMAND_ID&);
			bool sliderPopout(const COMMAND_ID& ID, GUI_METADATA& md);


	void renderMMBar();

	void render();

};