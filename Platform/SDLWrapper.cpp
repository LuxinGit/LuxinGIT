#include "SDLWrapper.h"
#include "Application/Application.h"
#include "imgui.h"

namespace SDL {

	namespace {

		void updateTexture( SDL_Texture* texture, const std::vector<luxel>& canvas, int width, int height) 
		{
			SDL_Rect rect{ 0, 0, width, height };
			SDL_UpdateTexture(texture, &rect, canvas.data(), DEFAULT_CANVAS_WIDTH_MAX * sizeof(luxel));
		}
		void renderTexture(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_FRect* src, const SDL_FRect& dest)
		{
			SDL_RenderTexture(renderer, texture, src, &dest);
		}

		void renderCanvasTexture(SDL_Renderer* rend, SDL_Texture* text, const Canvas_State& cS)
		{
			SDL_FRect rect{ 0.0f, 0.0f, static_cast<float>(cS.width), static_cast<float>(cS.height) };
			renderTexture(rend, text, &rect, rect);
		}
		SDL_FRect objectRect(const Object::object& o)
		{
			return {	static_cast<float>(o.topLeft.x), 
						static_cast<float>(o.topLeft.y), 
						static_cast<float>(o.bottomRight.x - o.topLeft.x + 1), 
						static_cast<float>(o.bottomRight.y - o.topLeft.y + 1) };
		}
		void renderCanvasOverlay(SDL_Renderer* renderer, int width, int height)
		{
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);

			SDL_FRect rect{ 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height) };
			SDL_RenderFillRect(renderer, &rect);
		}
		void renderCanvas(SDL_State& sS, const Canvas_State& cS, const Object_State& oS) 
		{
			updateTexture(sS.displayTexture, cS.displayCanvas, cS.width, cS.height);
			renderCanvasTexture(sS.Renderer, sS.displayTexture, cS);

			for (const auto& obj : oS.objects)			
				renderTexture(sS.Renderer, obj.texture, nullptr, obj.area);			

			if (cS.activeCanvas == &cS.bufferCanvas) 
			{
				renderCanvasOverlay(sS.Renderer, cS.width, cS.height);
				updateTexture(sS.bufferTexture, cS.bufferCanvas, cS.width, cS.height);
				renderCanvasTexture(sS.Renderer, sS.bufferTexture, cS);
			}
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


	void initialiseSDL(SDL_State& sS, Canvas_State& cs) 
	{
		sS.Window = SDL_CreateWindow(DEFAULT_APPLICATION_NAME, cs.width, cs.height, 0);
		sS.Renderer = SDL_CreateRenderer(sS.Window, nullptr);
		sS.displayTexture = createTexture(sS);
		sS.bufferTexture = createTexture(sS);
		SDL_SetTextureBlendMode(sS.bufferTexture, SDL_BLENDMODE_BLEND);
		SDL_SetWindowRelativeMouseMode(sS.Window, false);
	}

	SDL_Texture* createTexture(const SDL_State& sS, int w, int h)
	{
		return SDL_CreateTexture(sS.Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, w, h);
	}

	void renderFrame(Application_State& s) {
		Input::GUI::beginFrame();
		Cursor::checkCursorData(s);
		renderCanvas(s.SDLState, s.CanvasState, s.ObjectState);
		renderCrosshair	(s.SDLState, s.CursorState,s.DrawState); // todo  fix the fact that we be rendering the crosshair regardless of whether or not we be using the gui
		Input::GUI::renderGUI(s);
		SDL_RenderPresent(s.SDLState.Renderer);
	}

	void cleanup(SDL_State& s) {
		if (s.displayTexture)	SDL_DestroyTexture(s.displayTexture);
		if (s.Renderer) SDL_DestroyRenderer(s.Renderer);
		if (s.Window)	SDL_DestroyWindow(s.Window);
		SDL_Quit();
	}

}