#pragma once
#include <SDL3/SDL.h>
#include "STRUCTS.h"
#include <vector>

struct Master_Handler;
struct Canvas_State;

struct SDL_State {

    SDL_Window*     Window = nullptr;
    SDL_Renderer* Renderer = nullptr;
    SDL_Texture*   Texture = nullptr;

};

namespace SDL {

    void initialiseSDL(SDL_State&, Canvas_State&);

    void renderFrame(Master_Handler&);

    void cleanup(SDL_State&);

}