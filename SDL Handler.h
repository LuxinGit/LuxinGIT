#pragma once
#include <SDL3/SDL.h>

#include <vector>
struct luxel;

struct SDL_Handler {

    SDL_Window* Window;
    SDL_Renderer* Renderer;
    SDL_Texture* Texture;

    SDL_Handler();

    ~SDL_Handler();

    int initialiseSDL(int width, int height, const std::vector<luxel>& canvas);

    void refreshPresent(const std::vector<luxel>& canvas, int width) const;

    void cleanup() const;

private:

    void updateTexture(const std::vector<luxel>& canvas, int width) const;

    void renderTexture() const;
    void renderPresent() const;

    int initialiseWindow(int width, int height);

    int initialiseRenderer();

    int initialiseTexture(int width, int height);
};
