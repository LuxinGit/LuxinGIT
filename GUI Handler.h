#pragma once
#include "COMMAND.h"
#include <imgui.h>
#include <string>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include "SDL Handler.h"

struct Master_Handler;

struct GUI_Handler {

	inline static SDL_Window*&		Window		= SDL_Handler::Window;
	inline static SDL_Renderer*&	Renderer	= SDL_Handler::Renderer;

	std::array<std::vector<COMMAND_ID>, static_cast<int>(GUI_METADATA::HEADER::COUNT)> headers;
	std::unordered_map<COMMAND_ID, const GUI_METADATA*> labelMapping;

	Master_Handler& MasterHandler;

	GUI_Handler(Master_Handler& varMH);

	void initialise();

	void initialiseBinding(const Command_Definition& definition);

	void beginFrame();

	void menuItem(COMMAND_ID ID);

	void binaryMenuItem(COMMAND_ID ID, const GUI_METADATA* md);

	void sliderMenuItem(COMMAND_ID, const GUI_METADATA* md);

	void menu(const std::string& name, const std::vector<COMMAND_ID>& commands);

	void renderMMBar();

	bool wantsMouse();

	void render();

};