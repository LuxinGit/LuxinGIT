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

    void renderFrame(Application_State&);

    void cleanup(SDL_State&);

}