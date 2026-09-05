#include "SDLWrapper.h"
#include "Application/Application.h"
#include "imgui.h"

namespace SDL {

	namespace {
		void updateTexture(SDL_State& sS, Canvas_State& cS) {
			SDL_Rect rect{ 0, 0, cS.width, cS.height };
			SDL_UpdateTexture(sS.Texture, &rect, cS.canvas.data(), DEFAULT_CANVAS_WIDTH_MAX * sizeof(luxel));
		}
		void renderTexture(SDL_State& sS, Canvas_State& cS) {
			SDL_FRect rect{ 0, 0, static_cast<float>(cS.width), static_cast<float>(cS.height) };
			SDL_RenderTexture(sS.Renderer, sS.Texture, &rect, nullptr);
		}
		void renderCrosshair(SDL_State& sS, Cursor_State& cS, Draw_State& dS) {
			if (cS.enableCrosshair)
			{
				auto [x, y] = cS.cursor;
				int r = dS.pen;
				SDL_RenderLine(sS.Renderer, x - r, y, x + r, y);
				SDL_RenderLine(sS.Renderer, x, y - r, x, y + r);
			}
		};
	}


	void initialiseSDL(SDL_State& ss, Canvas_State& cs) 
	{
		ss.Window = SDL_CreateWindow(DEFAULT_APPLICATION_NAME, cs.width, cs.height, 0);
		ss.Renderer = SDL_CreateRenderer(ss.Window, nullptr);
		ss.Texture = createTexture(ss);
		SDL_SetWindowRelativeMouseMode(ss.Window, false);
	}

	SDL_Texture* createTexture(SDL_State& ss, int w, int h)
	{
		return SDL_CreateTexture(ss.Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, w, h);
	}

	void renderFrame(Application_State& s) {
		Input::GUI::beginFrame();
		Cursor::checkCursorData(s);
		updateTexture	(s.SDLState, s.CanvasState);
		renderTexture	(s.SDLState, s.CanvasState);
		renderCrosshair	(s.SDLState, s.CursorState,s.DrawState); // todo  fix the fact that we be rendering the crosshair regardless of whether or not we be using the gui
		Input::GUI::renderGUI(s);
		SDL_RenderPresent(s.SDLState.Renderer);
	}

	void cleanup(SDL_State& s) {
		if (s.Texture)	SDL_DestroyTexture(s.Texture);
		if (s.Renderer) SDL_DestroyRenderer(s.Renderer);
		if (s.Window)	SDL_DestroyWindow(s.Window);
		SDL_Quit();
	}

}