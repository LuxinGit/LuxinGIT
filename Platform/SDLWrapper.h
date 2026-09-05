#pragma once
#include <SDL3/SDL.h>
#include "Application/STRUCTS.h"
#include <vector>

struct Application_State;
struct Canvas_State;

struct SDL_State {

    SDL_Window*     Window = nullptr;
    SDL_Renderer* Renderer = nullptr;
    SDL_Texture*   Texture = nullptr;

};

namespace SDL {

    void initialiseSDL(SDL_State&, Canvas_State&);

    SDL_Texture* createTexture(SDL_State&, int w = DEFAULT_CANVAS_WIDTH_MAX, int h = DEFAULT_CANVAS_HEIGHT_MAX);

    void renderFrame(Application_State&);

    void cleanup(SDL_State&);

}