#include "SDL Handler.h"
#include "Canvas Handler.h"


SDL_Handler::SDL_Handler(Canvas_Handler& varCanvH) : CanvasHandler(varCanvH) {
    Window = nullptr;
    Renderer = nullptr;
    Texture = nullptr; // if adding multiple this will need to be rethought, maybe map?
}
SDL_Handler::~SDL_Handler() {
    cleanup();
}

int SDL_Handler::initialiseSDL() {
    if (initialiseWindow()) return 1;
    if (initialiseRenderer()) return 1;
    if (initialiseTexture()) return 1;
    SDL_UpdateTexture(Texture, nullptr, CanvasHandler.canvas.data(), CanvasHandler.width * sizeof(luxel));
    return 0;
}

void SDL_Handler::cleanup() const {
    if (Texture) SDL_DestroyTexture(Texture);
    if (Renderer) SDL_DestroyRenderer(Renderer);
    if (Window) SDL_DestroyWindow(Window);
    SDL_Quit();
}

void SDL_Handler::updateTexture() const {
    SDL_UpdateTexture(Texture, nullptr, CanvasHandler.canvas.data(), CanvasHandler.width * sizeof(luxel));
}
void SDL_Handler::renderTexture() const {
    SDL_RenderTexture(Renderer, Texture, nullptr, nullptr);
}
void SDL_Handler::renderPresent() const {
    SDL_RenderPresent(Renderer);
}
void SDL_Handler::renderCrosshair(const std::pair<float, float>& c, const int& r) const {
    SDL_RenderLine(Renderer, c.first - r, c.second, c.first + r, c.second);
    SDL_RenderLine(Renderer, c.first, c.second - r, c.first, c.second + r);
};
void SDL_Handler::refreshPresent() {
    CanvasHandler.CursorHandler.refreshCursor();
    updateTexture();
    renderTexture();
    if (CanvasHandler.CursorHandler.enableCrosshair) renderCrosshair(CanvasHandler.CursorHandler.cursor, CanvasHandler.CursorHandler.crosshairRadius);
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
