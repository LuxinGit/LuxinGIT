#pragma once
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include "SDL Handler.h"

struct Master_Handler;

struct GUI_Handler {

	inline static SDL_Window*&		Window		= SDL_Handler::Window;
	inline static SDL_Renderer*&	Renderer	= SDL_Handler::Renderer;

	Master_Handler& MasterHandler;

	GUI_Handler(Master_Handler& varMH);

	void initialise();

	void beginFrame();

	void render();

};