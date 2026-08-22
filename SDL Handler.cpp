#include "SDL Handler.h"
#include "Canvas Handler.h"


SDL_Handler::SDL_Handler(Canvas_Handler& varCanvH)
    : CanvasHandler(varCanvH) { initialiseSDL(); }
SDL_Handler::~SDL_Handler() {
    cleanup();
}

void SDL_Handler::initialiseSDL() {
    if (initialiseWindow()) return;
    if (initialiseRenderer()) return;
    if (initialiseTexture()) return;
    SDL_UpdateTexture(Texture, nullptr, CanvasHandler.canvas.data(), CanvasHandler.width * sizeof(luxel));

}

void SDL_Handler::cleanup() const {
    if (Texture) SDL_DestroyTexture(Texture);
    if (Renderer) SDL_DestroyRenderer(Renderer);
    if (Window) SDL_DestroyWindow(Window);
    SDL_Quit();
}

void SDL_Handler::updateCanvasTexture() const {
    SDL_UpdateTexture(Texture, nullptr, CanvasHandler.canvas.data(), CanvasHandler.width * sizeof(luxel));
}
void SDL_Handler::renderTexture() const {
    SDL_RenderTexture(Renderer, Texture, nullptr, nullptr);
}
void SDL_Handler::renderPresent() const {
    SDL_RenderPresent(Renderer);
}
void SDL_Handler::renderCrosshair() const {
    if (CanvasHandler.CursorHandler.enableCrosshair)
    {
        auto [x, y] = CanvasHandler.CursorHandler.cursor;
        int r = CanvasHandler.CursorHandler.crosshairRadius;
        SDL_RenderLine(Renderer, x - r, y, x + r, y);
        SDL_RenderLine(Renderer, x, y - r, x, y + r);
    }
};
void SDL_Handler::refreshPresent() {
    CanvasHandler.CursorHandler.refreshCursor();
    updateCanvasTexture();
    renderTexture();
    renderCrosshair();
    renderPresent();
}

int SDL_Handler::initialiseWindow() {
    if (!SDL_Init(SDL_INIT_VIDEO))
        return 1;

    Window = SDL_CreateWindow("Better Paint", CanvasHandler.width, CanvasHandler.height, 0);
    if (!Window) return 1; else return 0;
}
int SDL_Handler::initialiseRenderer() {
    Renderer = SDL_CreateRenderer(Window, nullptr);
    if (!Renderer) return 1; else return 0;
}
int SDL_Handler::initialiseTexture() {
    Texture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, CanvasHandler.width, CanvasHandler.height);
    if (!Texture) return 1; else return 0;
}
