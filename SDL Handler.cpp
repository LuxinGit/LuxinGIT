#include "SDL Handler.h"
#include "Canvas Handler.h"


SDL_Handler::SDL_Handler() {
    Window = nullptr;
    Renderer = nullptr;
    Texture = nullptr; // if adding multiple this will need to be rethought, maybe map?
}
SDL_Handler::~SDL_Handler() {
    cleanup();
}

int SDL_Handler::initialiseSDL(int width, int height, const std::vector<luxel>& canvas) {
    if (initialiseWindow(width, height)) return 1;
    if (initialiseRenderer()) return 1;
    if (initialiseTexture(width, height)) return 1;
    SDL_UpdateTexture(Texture, nullptr, canvas.data(), width * sizeof(luxel));
    return 0;
}

void SDL_Handler::cleanup() const {
    if (Texture) SDL_DestroyTexture(Texture);
    if (Renderer) SDL_DestroyRenderer(Renderer);
    if (Window) SDL_DestroyWindow(Window);
    SDL_Quit();
}

void SDL_Handler::updateTexture(const std::vector<luxel>& canvas, int width) const {
    SDL_UpdateTexture(Texture, nullptr, canvas.data(), width * sizeof(luxel));
}
void SDL_Handler::renderTexture() const {
    SDL_RenderTexture(Renderer, Texture, nullptr, nullptr);
}
void SDL_Handler::renderPresent() const {
    SDL_RenderPresent(Renderer);
}
void SDL_Handler::refreshPresent(const std::vector<luxel>& canvas, int width) const {
    updateTexture(canvas, width);
    renderTexture();
    renderPresent();
}

int SDL_Handler::initialiseWindow(int width, int height) {
    if (!SDL_Init(SDL_INIT_VIDEO))
        return 1;

    Window = SDL_CreateWindow("Better Paint", width, height, 0);
    if (!Window) return 1; else return 0;
}
int SDL_Handler::initialiseRenderer() {
    Renderer = SDL_CreateRenderer(Window, nullptr);
    if (!Renderer) return 1; else return 0;
}
int SDL_Handler::initialiseTexture(int width, int height) {
    Texture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!Texture) return 1; else return 0;
}

