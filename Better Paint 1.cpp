// Better Paint 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL3/SDL.h>
#include <vector>
#include <array>

/*
TO DO:

1. Central Command - data structure that owns everything created (split into 3 sections - settings, canvas management, SDL managment).
2. SDL_GetKeyboardState to allow for simultaneous inputs.
3. Text based interface?
4. Boundary checking.



*/


#pragma region Luxels
struct luxel {
    std::array<uint8_t, 4> colour;

    luxel() { colour = { 255, 255, 255, 255 }; } // 0 = R, 1 = G, 2 = B, 3 = Opacity
    

    void resetLuxel() {
        colour = { 255, 255, 255, 255 };
    }

};
#pragma endregion

#pragma region Canvas Handler
struct Canvas_Handler {

private: 

    int width; int height;

    std::vector<luxel> canvas;

public:

    Canvas_Handler(int varwidth, int varheight) {
        width = varwidth; height = varheight;
        canvas = std::vector<luxel>(width * height);
    }

    void clearCanvas() {
        for (luxel& l : canvas) {
            l.resetLuxel();
        }
    }

    std::vector<luxel> retrieveCanvas() { return canvas; }

private:

    size_t indexFromCoord(std::pair<float, float> c) const {
        // ASSUMES POSITIVE X/Y. INDEXING WITH THIS INDEX WITHOUT SIZE CHECKING MAY CAUSE OUT OF BOUNDARY MEMORY CRASH [IF COORD > LAST LUXEL INDEX].
        return size_t(c.second) * width + size_t(c.first);
    }

    void drawPoint(std::pair<float, float> c, const std::array<uint8_t, 4>& colour) {
        canvas[indexFromCoord(c)].colour = colour;
    }

    void drawVerticalLine(std::pair<float, float>& c, int y, const std::array<uint8_t, 4>& colour) {
        bool n = y < c.second;
        int d = 1; if (n) d = -1;
        for (c.second; c.second != y; c.second += d) drawPoint(c, colour);
        drawPoint({ c.first, y }, colour);
        return;
    }

public:

    void drawLine(std::pair<float, float>& c, std::pair<float, float> newc, const std::array<uint8_t, 4>& colour) {

        drawPoint(c, colour);

        // vertical line edge case
        if (c.first == newc.first) {
            drawVerticalLine(c, newc.second, colour);
            return;
        }

        // Figure out direction (lines can be backwards)
        bool n = newc.first < c.first;
        int d = 1; if (n) d = -1;

        float slope = (newc.second - c.second) / (newc.first - c.first);
        float y = 0.0f;

        for (c.first; c.first != newc.first; c.first += d) {
            y = c.second + slope;
            c.second = y;
            drawPoint(c, colour);
        }

        // At ending x
        drawPoint(newc, colour);

        return;
    }
    void drawLine(std::pair<std::pair<float, float>, std::pair<float, float>>& payload, const std::array<uint8_t, 4>& colour) {
        drawLine(payload.first, payload.second, colour);
    }
};
#pragma endregion

#pragma region Cursor Handler
struct Cursor_Handler {

private:

    std::pair<float, float> cursor;
    std::pair<float, float> newCursor;
    std::pair<float, float> deltaCursor = { 0, 0 }; // newCursor = cursor + deltaCursor at t==0, cursor = newCursor at t==1;
    std::pair<std::pair<float, float>, std::pair<float,float>> bothCursors; // for sending to drawLine;

    int drawStep = 10;

    void adjustDeltaCursor(std::pair<float, float> incoming) { deltaCursor.first += incoming.first; deltaCursor.second += incoming.second; }

public:

    Cursor_Handler(std::pair<float, float> varcursor) { cursor = varcursor; newCursor = varcursor; };

    void updateDrawstep(int delta) { 
        drawStep += delta;
        if (drawStep < 5) drawStep = 5;
    } // probably should sizecheck this but we can leave that for now
    
    void updateDeltaCursor(bool yaxis, bool negative) {
        std::pair<float, float> payload = { 0,0 };
        int tempstep = drawStep;
        if (negative) tempstep *= -1;

        if (yaxis) payload.second += tempstep;
        else payload.first += tempstep;

        adjustDeltaCursor(payload);
    }

    void calcNewCursor() { 
        newCursor.first += deltaCursor.first; 
        newCursor.second += deltaCursor.second; 
        bothCursors = { cursor, newCursor };
    }

    void resetCursors() {
        cursor = newCursor;
        deltaCursor = { 0 , 0 };
    }

    std::pair<std::pair<float, float>, std::pair<float, float>>& retrieveBothCursors() { 
        return bothCursors;
    }
};
#pragma endregion

#pragma region SDL Handler
struct SDL_Handler {

    SDL_Window* Window;
    SDL_Renderer* Renderer;
    SDL_Texture* Texture;

    SDL_Handler() {
        Window = nullptr;
        Renderer = nullptr;
        Texture = nullptr; // if adding multiple this will need to be rethought, maybe map?
    }

    ~SDL_Handler() {
        cleanup();
    }

private:

    int initialiseWindow(int width, int height) {
        if (!SDL_Init(SDL_INIT_VIDEO))
            return 1;

        Window = SDL_CreateWindow("Better Paint", width, height, 0);
        if (!Window) return 1; else return 0;
    }

    int initialiseRenderer() {
        Renderer = SDL_CreateRenderer(Window, nullptr);
        if (!Renderer) return 1; else return 0;
    }

    int initialiseTexture(int width, int height) {
        Texture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
        if (!Texture) return 1; else return 0;
    }

public:

    int initialiseSDL(int width, int height) {
        if (initialiseWindow(width, height)) return 1;
        if (initialiseRenderer()) return 1;
        if (initialiseTexture(width, height)) return 1;
        return 0;
    }

    void updateTexture(const std::vector<luxel>& canvas, int width) const  {
        SDL_UpdateTexture(Texture, nullptr, canvas.data(), width * sizeof(luxel));
    }

    void renderTexture() const { 
        SDL_RenderTexture(Renderer, Texture, nullptr, nullptr);  
    }

    void renderPresent() const {
        SDL_RenderPresent(Renderer);
    }

    void cleanup() const {
        if (Texture) SDL_DestroyTexture(Texture);
        if (Renderer) SDL_DestroyRenderer(Renderer);
        if (Window) SDL_DestroyWindow(Window);
        SDL_Quit();
    }

};
#pragma endregion

#pragma region Master Handler

#pragma endregion

int main()
{

    // DEFINES
    int width = 800, height = 600;
    bool running = true;
    std::array<uint8_t, 4> colour = { 200, 200, 200, 255 };

    SDL_Handler SDLHandler;
    Cursor_Handler CursorHandler({ 100.0f, 100.0f });
    Canvas_Handler CanvasHandler(width, height);

    if (SDLHandler.initialiseSDL(width, height)) return 1;
    
    SDLHandler.updateTexture(CanvasHandler.retrieveCanvas(), width);

    while (running) {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                    case SDLK_W:
                        CursorHandler.updateDeltaCursor(true, true);
                        break;
                    case SDLK_S:
                        CursorHandler.updateDeltaCursor(true, false);
                        break;
                    case SDLK_A:
                        CursorHandler.updateDeltaCursor(false, true);
                        break;
                    case SDLK_D:
                        CursorHandler.updateDeltaCursor(false, false);
                        break;
                    case SDLK_Z:
                        CursorHandler.updateDrawstep(-5);
                        break;
                    case SDLK_X:
                        CursorHandler.updateDrawstep(5);
                        break;
                    case SDLK_C:
                        CanvasHandler.clearCanvas();
                        break;
                }
            }
            
        }

        CursorHandler.calcNewCursor();

        CanvasHandler.drawLine(CursorHandler.retrieveBothCursors(), colour);
        
        CursorHandler.resetCursors();

        SDLHandler.updateTexture(CanvasHandler.retrieveCanvas(), width);
        SDLHandler.renderTexture();
        SDLHandler.renderPresent();

    }

    // Clean up
    SDLHandler.cleanup();
    return 0;
}