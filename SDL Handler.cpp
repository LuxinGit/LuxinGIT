#include "SDL Handler.h"
#include "Canvas Handler.h"


SDL_Handler::SDL_Handler(Canvas_Handler& varCanvH)
    : canvas(varCanvH.canvas), canvasHeight(varCanvH.height), canvasWidth(varCanvH.width), CursorHandler(varCanvH.CursorHandler) { initialiseSDL(); }
SDL_Handler::~SDL_Handler() {
    cleanup();
}

void SDL_Handler::initialiseSDL() {
    Window = SDL_CreateWindow(DEFAULT_APPLICATION_NAME, canvasWidth, canvasHeight, 0);
    Renderer = SDL_CreateRenderer(Window, nullptr);
    Texture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, canvasWidth, canvasHeight);
    updateCanvasTexture(Texture, canvas, canvasWidth * sizeof(luxel));
}

void SDL_Handler::cleanup() const {
    if (Texture) SDL_DestroyTexture(Texture);
    if (Renderer) SDL_DestroyRenderer(Renderer);
    if (Window) SDL_DestroyWindow(Window);
    SDL_Quit();
}

void SDL_Handler::updateCanvasTexture(SDL_Texture* texture, std::vector<luxel>& canvas, int pitch) {
    SDL_UpdateTexture(texture, nullptr, canvas.data(), pitch);
}
void SDL_Handler::renderTexture(SDL_Texture* texture) {
    SDL_RenderTexture(Renderer, texture, nullptr, nullptr);
}
void SDL_Handler::renderPresent() const {
    SDL_RenderPresent(Renderer);
}
void SDL_Handler::renderCrosshair() const {
    if (CursorHandler.enableCrosshair)
    {
        auto [x, y] = CursorHandler.cursor;
        int r = CursorHandler.CanvasHandler.DrawHandler.pen;
        SDL_RenderLine(Renderer, x - r, y, x + r, y);
        SDL_RenderLine(Renderer, x, y - r, x, y + r);
    }
};
void SDL_Handler::refreshPresent() {
    CursorHandler.refreshCursor();
    updateCanvasTexture(Texture, canvas, canvasWidth * sizeof(luxel));
    renderTexture(Texture);
    renderCrosshair();
    renderPresent();
}
