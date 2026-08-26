#pragma once
#include <SDL3/SDL.h>

#include <vector>

struct luxel;
struct Canvas_Handler;
struct Cursor_Handler;

struct SDL_Handler {

    inline static SDL_Window*      Window;
    inline static SDL_Renderer*    Renderer;
    SDL_Texture* Texture =      nullptr;

    std::vector<luxel>& canvas;
    int& canvasWidth, canvasHeight;
    Cursor_Handler& CursorHandler;

    SDL_Handler(Canvas_Handler& varCanvH);

    ~SDL_Handler();

    void initialiseSDL();

    void refreshPresent();
    
    void cleanup() const;

private:

    static void updateCanvasTexture(SDL_Texture* texture, std::vector<luxel>& canvas, int pitch);
    static void renderTexture(SDL_Texture* texture);
    void renderCrosshair() const;
    void renderPresent() const;

};
