// Better Paint 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL3/SDL.h>
#include <vector>
#include <array>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <map>

/*
TO DO:

1. Refocus command execution loop (to be done in conjunction with below
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

    int& width; int& height;
    std::array<uint8_t, 4> colour = { 200, 200, 200, 255 };
    std::vector<luxel> canvas;

    size_t indexFromCoord(std::pair<float, float> c) const {
        // ASSUMES POSITIVE X/Y. INDEXING WITH THIS INDEX WITHOUT SIZE CHECKING MAY CAUSE OUT OF BOUNDARY MEMORY CRASH [IF COORD > LAST LUXEL INDEX].
        return size_t(c.second) * width + size_t(c.first);
    }

    void drawPoint(std::pair<float, float> c) {
        canvas[indexFromCoord(c)].colour = colour;
    }

    void drawVerticalLine(std::pair<float, float>& c, float y) {
        bool n = y < c.second;
        int d = 1; if (n) d = -1;
        for (c.second; c.second != y; c.second += d) drawPoint(c);
        drawPoint({ c.first, y });
        return;
    }

    void drawCcl(const std::pair<float, float>& c, int radius) {
        int offset = 0;
        for (int i = c.first - radius; i <= c.first + radius; i++) {
            offset = static_cast<int>(std::sqrt(radius * radius - (i - c.first) * (i - c.first)));
            drawPoint({ i,c.second + offset }); drawPoint({ i, c.second - offset });
        }
    }


public:

    Canvas_Handler(int& varwidth, int& varheight) : width(varwidth), height(varheight), canvas(std::vector<luxel>(width * height)) {
       // Only concern here is that if underlying height / width changes then this will need to be recalculated.
    }

    void clearCanvas() {
        for (luxel& l : canvas) {
            l.resetLuxel();
        }
    }

    std::vector<luxel> retrieveCanvas() { return canvas; }
    int& retrieveCanvasWidth() { return width; }

    void changeColour(const std::array<uint8_t, 4>& varcolour) { colour = varcolour; };

    void drawLine(std::pair<float, float>& c, std::pair<float, float> newc) {

        drawPoint(c);

        // vertical line edge case
        if (c.first == newc.first) {
            drawVerticalLine(c, newc.second);
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
            drawPoint(c);
        }

        // At ending x
        drawPoint(newc);

        return;
    }
    void drawLine(std::pair<std::pair<float, float>, std::pair<float, float>>& payload) {
        drawLine(payload.first, payload.second);
    }

    void drawCircle(const std::pair<float, float>& c, int radius, bool setting) {
        if (setting) {
            for (int i = 1; i <= radius; i++) {
                colour = { static_cast<uint8_t>(rand() % 256),static_cast<uint8_t>(rand() % 256),static_cast<uint8_t>(rand() % 256), 255 };
                drawCcl(c, i);
            }
            colour = { 200, 200, 200, 255 };
            return;
        }
        else drawCcl(c, radius);
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

    Cursor_Handler() { cursor = { 100, 100 }; newCursor = cursor; }
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

    std::pair<float, float> retrieveCursor() const { return cursor; }
    int retrieveDrawstep() const { return drawStep; }
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

    int initialiseSDL(int width, int height, const std::vector<luxel>& canvas) {
        if (initialiseWindow(width, height)) return 1;
        if (initialiseRenderer()) return 1;
        if (initialiseTexture(width, height)) return 1;
        SDL_UpdateTexture(Texture, nullptr, canvas.data(), width * sizeof(luxel));
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
};
#pragma endregion

#pragma region Commands

struct Command {

public:

    enum class TYPE {
        DRAW = 0,
        MOVE = 1,
        META = 2
    };

private:

    Command(TYPE vartype, int varaction, int varsetting = 0) : type(vartype), action(varaction), setting(varsetting) {}

public:

    enum class MOVE {
        UP = 0,
        RIGHT = 1,
        DOWN = 2,
        LEFT = 3
    };

    enum class DRAW {
        LINE = 0,
        CIRCLE = 1
    };

    enum class META {
        CLEAR = 0
    };

    Command(MOVE cmd, int varsetting = 0) : Command(TYPE::MOVE, static_cast<int>(cmd), varsetting) {}
    Command(DRAW cmd, int varsetting = 0) : Command(TYPE::DRAW, static_cast<int>(cmd), varsetting) {}
    Command(META cmd, int varsetting = 0) : Command(TYPE::META, static_cast<int>(cmd), varsetting) {}

    TYPE type;
    int action; // differentiates between commands.
    int setting = 0; // for passing additional information ?

};


#pragma endregion

#pragma region Command Handler

struct Command_Handler {

private:

    std::vector<Command> commandQueue;

    // References to handlers needed for command processing
    Cursor_Handler& CursorHandler;
    Canvas_Handler& CanvasHandler;
    SDL_Handler& SDLHandler;

    // IDEALLY MOVE BELOW TO SEPARATE HANDLERS.

    void updateSDLTexture() {
        SDLHandler.updateTexture(CanvasHandler.retrieveCanvas(), CanvasHandler.retrieveCanvasWidth());
    }
    void renderNewSDLTexture() {
        updateSDLTexture();
        SDLHandler.renderTexture();
        SDLHandler.renderPresent();
    }

    void drawLine() {
        CursorHandler.calcNewCursor();
        CanvasHandler.drawLine(CursorHandler.retrieveBothCursors());
        CursorHandler.resetCursors();
    }
    void drawCircle(bool setting = false) {
        CanvasHandler.drawCircle(CursorHandler.retrieveCursor(), CursorHandler.retrieveDrawstep(), setting);
    }

    void processDrawCommand(const Command& command) {
        switch (static_cast<Command::DRAW>(command.action)) {
        case Command::DRAW::LINE:
            drawLine();
            break;
        case Command::DRAW::CIRCLE:
            drawCircle(command.setting);
            break;
        }
    };
    void processMoveCommand(const Command& command) { // not in use
        switch (static_cast<Command::MOVE>(command.action)) {
        case Command::MOVE::UP:
            CanvasHandler.clearCanvas();
            break;
        default:
            break;
        }

    };
    void processMetaCommand(const Command& command) {
        switch (static_cast<Command::META>(command.action)) {
        case Command::META::CLEAR:
            CanvasHandler.clearCanvas();
            break;
        }
    };

    // ON THE BASIS THAT THE ABOVE IS LARGELY HANDLED BY RESPECTIVE HANDLERS AND THIS SHOULDNT BE RESPONSIBLE.

    void processCommand(const Command& command) {
        switch (command.type) {
        case Command::TYPE::DRAW:
            processDrawCommand(command);
            break;
        case Command::TYPE::MOVE:
            processMoveCommand(command);
            break;
        case Command::TYPE::META:
            processMetaCommand(command);
            break;
        }
    }


public:

    void processCommands() {
        for (const auto& command : commandQueue) processCommand(command);
        renderNewSDLTexture();
        clearCommands();
    }

    void addCommand(Command command) {
        commandQueue.emplace_back(command);
    }
    void clearCommands() { commandQueue = {}; }

    Command_Handler(Cursor_Handler& varCursH, Canvas_Handler& varCanvH, SDL_Handler& varSDLH)
        : CursorHandler(varCursH), CanvasHandler(varCanvH), SDLHandler(varSDLH) {}

};
#pragma endregion

#pragma region Keyboard Handler
struct Keyboard_Handler {

private:

    

public:

};
#pragma endregion

#pragma region Master Handler

struct Master_Handler {

private:

    int width; int height;

    Cursor_Handler CursorHandler;
    Canvas_Handler CanvasHandler;
    SDL_Handler SDLHandler;
    Command_Handler CommandHandler;

public: 

    Master_Handler(int varwidth, int varheight) : 
        width(varwidth), height(varheight), 
        CanvasHandler(width, height), 
        CommandHandler(CursorHandler, CanvasHandler, SDLHandler) {};

    int initialiseSDL() {
        return (SDLHandler.initialiseSDL(width, height, CanvasHandler.retrieveCanvas()));
    }

    void processCommands() {
        CommandHandler.processCommands();
    }
    void addCommand(Command command) {
        CommandHandler.addCommand(command);
    }
    void updateDrawstep(int delta) {
        return CursorHandler.updateDrawstep(delta);
    }

    void processCursorMovement(const std::pair<bool, bool>& yandneg) {
        CursorHandler.updateDeltaCursor(yandneg.first, yandneg.second);
        addCommand(Command{Command::DRAW::LINE, 0});
    }

    void cleanup() const {
        SDLHandler.cleanup();
    }

};

#pragma endregion

#pragma region Main
int main()
{

    // DEFINES
    int width = 800, height = 600;
    bool running = true;

    Master_Handler MasterHandler(width, height);

    if (MasterHandler.initialiseSDL()) return 1;

    while (running) {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                    case SDLK_W:
                        MasterHandler.processCursorMovement({ true,true });
                        break;
                    case SDLK_S:
                        MasterHandler.processCursorMovement({ true,false });
                        break;
                    case SDLK_A:
                        MasterHandler.processCursorMovement({ false,true });
                        break;
                    case SDLK_D:
                        MasterHandler.processCursorMovement({ false,false });
                        break;
                    case SDLK_G:
                        MasterHandler.addCommand(Command{Command::DRAW::CIRCLE});
                        break;
                    case SDLK_L:
                        MasterHandler.addCommand(Command{Command::DRAW::CIRCLE, 1});
                        break;
                    case SDLK_Z:
                        MasterHandler.updateDrawstep(-5);
                        break;
                    case SDLK_X:
                        MasterHandler.updateDrawstep(5);
                        break;
                    case SDLK_C:
                        MasterHandler.addCommand(Command{Command::META::CLEAR});
                        break;
                }
            }
            
        }

        MasterHandler.processCommands();

    }

    // Clean up
    MasterHandler.cleanup();
    return 0;
}
#pragma endregion