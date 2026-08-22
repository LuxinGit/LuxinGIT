#pragma once
#include <SDL3/SDL.h>

#include <vector>

struct luxel;
struct Canvas_Handler;

struct SDL_Handler {

    SDL_Window* Window;
    SDL_Renderer* Renderer;
    SDL_Texture* Texture;

    Canvas_Handler& CanvasHandler;

    SDL_Handler(Canvas_Handler& varCanvH);

    ~SDL_Handler();

    int initialiseSDL();

    void refreshPresent();
    
    void cleanup() const;

private:

    void updateCanvasTexture() const;
    void renderTexture() const;
    void renderCrosshair() const;
    void renderPresent() const;

    int initialiseWindow();
    int initialiseRenderer();
    int initialiseTexture();
};
