#pragma once
#include <SDL3/SDL.h>

#include <vector>

struct luxel;
struct Master_Handler;
struct Canvas_Handler;
struct Cursor_Handler;
struct GUI_Handler;

struct SDL_Handler {

    inline static SDL_Window*      Window;
    inline static SDL_Renderer*    Renderer;
    SDL_Texture* Texture =      nullptr;

    std::vector<luxel>& canvas;
    int& canvasWidth;
    int& canvasHeight;
    Cursor_Handler& CursorHandler;
    GUI_Handler* GUIHandler;
    bool* enableMouse;

    SDL_Handler(Master_Handler& varMH);

    ~SDL_Handler();

    void initialiseTexture();

    void registerCanvasSizeChange();

    void initialiseSDL(GUI_Handler& varGH, bool* varenableMouse);

    void renderFrame();
    
    void cleanup() const;

    void checkShowMouse() const;

private:

    static void updateCanvasTexture(SDL_Texture* texture, std::vector<luxel>& canvas, int pitch);
    static void renderTexture(SDL_Texture* texture);
    void renderCrosshair() const;
    void renderPresent() const;

};
