// Better Paint 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL3/SDL.h>
#include <vector>
#include <array>

#pragma region Quit
static int quit(int error) {
    SDL_Quit(); return error;
}
static int quit(int error, SDL_Window* window) {
    SDL_DestroyWindow(window);
    return quit(error);
}
static int quit(int error, SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    return quit(error, window);
}
static int quit(int error, SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_DestroyTexture(texture);
    return quit(error, window, renderer);
}
#pragma endregion

static SDL_Window* initialiseSDL(int width, int height) {

    if (!SDL_Init(SDL_INIT_VIDEO))
        return nullptr;

    SDL_Window* window = SDL_CreateWindow(
        "Better Paint",
        width, 
        height,
        0
    );

    return window;

}

#pragma region Luxels + Canvas
struct luxel {
    std::array<uint8_t, 4> colour;

    luxel() { colour = { 255, 255, 255, 255 }; } // 0 = R, 1 = G, 2 = B, 3 = Opacity
    

    void resetLuxel() {
        colour = { 255, 255, 255, 255 };
    }

};
static std::vector<luxel> createCanvas(int width, int height) {
    return std::vector<luxel>(width * height);
}
static void clearCanvas(std::vector<luxel>& canvas) {
    for (luxel& l : canvas) {
        l.resetLuxel();
    }
}
#pragma endregion

#pragma region Line Drawing
static size_t indexFromCoord(std::pair<float,float> c, int width) {
    // ASSUMES POSITIVE X/Y. INDEXING WITH THIS INDEX WITHOUT SIZE CHECKING MAY CAUSE OUT OF BOUNDARY MEMORY CRASH [IF COORD > LAST LUXEL INDEX].
    return size_t(c.second) * width + size_t(c.first);
}
static void drawPoint(std::vector<luxel>& canvas, int width, std::pair<float,float> c, const std::array<uint8_t, 4>& colour) {
    canvas[indexFromCoord(c, width)].colour = colour;
}
static void drawVerticalLine(std::vector<luxel>& canvas, int width, std::pair<float, float>& c, int y, const std::array<uint8_t, 4>& colour) {
    bool n = y < c.second;
    int d = 1; if (n) d = -1;
    for (c.second; c.second != y; c.second += d) drawPoint(canvas, width, c, colour);
    drawPoint(canvas, width, {c.first, y}, colour);
    return;
}
static void drawLine(std::vector<luxel>& canvas, int width, std::pair<float,float>& c, std::pair<float,float> newc, const std::array<uint8_t, 4>& colour) {
    
    drawPoint(canvas, width, c, colour);
    
    // vertical line edge case
    if (c.first == newc.first) { 
        drawVerticalLine(canvas, width, c, newc.second, colour);
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
        drawPoint(canvas, width, c, colour);
    }

    // At ending x
    drawPoint(canvas, width, newc, colour);

    return;
}
#pragma endregion

#pragma region Keyboard State Handling



#pragma endregion

int main()
{

    // DEFINES
    int width = 800, height = 600;
    bool running = true;
    std::pair<float, float> cursorC = { 100.0f, 100.0f }; //x,y
    std::pair<float, float> newCursorC = cursorC;
    std::pair<float, float> deltaC = { 0.0f, 0.0f }; // xDelta, yDelta
    int drawStep = 10;
    std::array<uint8_t, 4> colour = { 200, 200, 200, 255 };

    SDL_Window* window = initialiseSDL(width, height);
    if (!window) return quit(1);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) return quit(1, window);

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) return quit(1, window, renderer);

    std::vector<luxel> canvas = createCanvas(width, height);
    int canvasSize = width * sizeof(luxel);
    SDL_UpdateTexture(texture, nullptr, canvas.data(), canvasSize);

   

    while (running) {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                    case SDLK_W:
                        deltaC.second -= drawStep;
                        break;
                    case SDLK_S:
                        deltaC.second += drawStep;
                        break;
                    case SDLK_A:
                        deltaC.first -= drawStep;
                        break;
                    case SDLK_D:
                        deltaC.first += drawStep;
                        break;
                    case SDLK_Z:
                        drawStep -= 5;
                        if (drawStep < 5) drawStep = 5;
                        break;
                    case SDLK_X:
                        drawStep += 5;
                        break;
                    case SDLK_C:
                        clearCanvas(canvas);
                        break;
                }
            }
            
        }

        newCursorC.first = cursorC.first + deltaC.first; newCursorC.second = cursorC.second + deltaC.second;

        drawLine(canvas, width, cursorC, newCursorC, colour);
        cursorC = newCursorC;

        SDL_UpdateTexture(texture, nullptr, canvas.data(), canvasSize);

        SDL_RenderTexture(renderer, texture, nullptr, nullptr);

        SDL_RenderPresent(renderer);

        deltaC = { 0,0 };

    }

    // Clean up
    return quit(0, window, renderer, texture);
}