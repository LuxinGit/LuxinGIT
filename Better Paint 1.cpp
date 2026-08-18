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
#include <unordered_map>
/*
TO DO:


1. Text based interface?
2. Brush modes
3. Flesh out commands to make more complex


*/

struct Command;
struct Luxel;

struct Canvas_Handler;
struct SDL_Handler;
struct Command_Handler;
struct Master_Handler;

#pragma region Luxels
struct luxel {
    std::array<uint8_t, 4> colour;

    luxel() { colour = { 255, 255, 255, 255 }; } // 0 = R, 1 = G, 2 = B, 3 = Opacity
    

    void resetLuxel() {
        colour = { 255, 255, 255, 255 };
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
        CLEAR = 0,
        CHANGE_COLOUR = 1,
        CHANGE_DRAWSTEP = 2,
        ENABLE_RAINBOW = 3
    };

    Command(MOVE cmd, int varsetting = 0) : Command(TYPE::MOVE, static_cast<int>(cmd), varsetting) {}
    Command(DRAW cmd, int varsetting = 0) : Command(TYPE::DRAW, static_cast<int>(cmd), varsetting) {}
    Command(META cmd, int varsetting = 0) : Command(TYPE::META, static_cast<int>(cmd), varsetting) {}

    TYPE type;
    int action; // differentiates between commands.
    int setting = 0; // for passing additional information ?

};


#pragma endregion

#pragma region Canvas Handler
struct Canvas_Handler {

private: 

    int& width; int& height;
    std::vector<luxel> canvas;

    size_t indexFromCoord(std::pair<float, float> c) const {
        // ASSUMES POSITIVE X/Y. INDEXING WITH THIS INDEX WITHOUT SIZE CHECKING MAY CAUSE OUT OF BOUNDARY MEMORY CRASH [IF COORD > LAST LUXEL INDEX].
        return size_t(c.second) * width + size_t(c.first);
    }
    bool coordCheck(const std::pair<float, float>& c) {
        if (c.first < 0 or c.first > width) return false;
        if (c.second < 0 or c.second >= height) return false;
        return true;
    }

#pragma region Cursor Handler

    struct Cursor_Handler {

    private:

        Canvas_Handler& CanvasHandler;

        std::pair<float, float> cursor = { 100, 100 };
        std::pair<float, float> newCursor = { 100, 100 };
        std::pair<float, float> deltaCursor = { 0, 0 }; // newCursor = cursor + deltaCursor at t==0, cursor = newCursor at t==1;
        size_t pixelsDrawn = 0;

        int drawStep = 2;

        void adjustDeltaCursor(std::pair<float, float> incoming) { deltaCursor.first += incoming.first; deltaCursor.second += incoming.second; }
      
        void updateDrawstep(int delta) {
            drawStep += delta;
            if (drawStep < 2) drawStep = 2;
            if (drawStep > 10) drawStep = 10;
        } // probably should sizecheck this but we can leave that for now



    public:

        Cursor_Handler(Canvas_Handler& varCanvH) : CanvasHandler(varCanvH) {}


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
        }
        void resetCursors() {
            cursor = newCursor;
            deltaCursor = { 0 , 0 };
            if (!CanvasHandler.coordCheck(cursor)) {
                if (cursor.first < 0) cursor.first = 1;
                if (cursor.first > CanvasHandler.width) cursor.first = static_cast<int>(CanvasHandler.width - 1);
                if (cursor.second < 0) cursor.second = 1;
                if (cursor.second >= CanvasHandler.height) cursor.second = static_cast<int>(CanvasHandler.height - 1);
            }
        }

        void processChangeDrawstepCommand(int setting) {
            updateDrawstep(setting); // lazy !
        }
        void processMoveCommand(bool yaxis, bool negative, int setting = 0) {
            updateDeltaCursor(yaxis, negative);
            CanvasHandler.DrawHandler.processDrawLineCommand();
        }

        void updatePixelsDrawn(int delta) { pixelsDrawn += delta; }
        size_t& retrievePixelsDrawn() { return pixelsDrawn; }

        std::pair<float, float>& retrieveCursor() { return cursor; }
        std::pair<float, float>& retrieveNewCursor() { return newCursor; }
        int& retrieveDrawstep() { return drawStep; }


    };
#pragma endregion

    Cursor_Handler CursorHandler;

#pragma region Draw Handler

    struct Draw_Handler {

    private:

        Canvas_Handler& CanvasHandler;
        std::array<uint8_t, 4> colour = { 200, 200, 200, 255 };

        void drawPoint(std::pair<float, float> c) {
            if (!CanvasHandler.coordCheck(c)) return;
            CanvasHandler.retrieveCanvas()[CanvasHandler.indexFromCoord(c)].colour = colour;
            CanvasHandler.CursorHandler.updatePixelsDrawn(1);
            if (rainbowMode) if (CanvasHandler.CursorHandler.retrievePixelsDrawn() > pixelsToRainbow) colour = getRandomColour();
        }
        void drawCircle(const std::pair<float, float>& c, int radius) {
            int x = 0;
            int y = radius;
            int d = 1 - radius;

            while (x <= y) {

                drawPoint({ c.first + x, c.second + y });
                drawPoint({ c.first - x, c.second + y });
                drawPoint({ c.first + x, c.second - y });
                drawPoint({ c.first - x, c.second - y });

                drawPoint({ c.first + y, c.second + x });
                drawPoint({ c.first - y, c.second + x });
                drawPoint({ c.first + y, c.second - x });
                drawPoint({ c.first - y, c.second - x });

                x++;

                if (d < 0) {
                    d += 2 * x + 1;
                }
                else {
                    y--;
                    d += 2 * (x - y) + 1;
                }
            }
        }

        bool rainbowMode = false; int pixelsToRainbow = 0;



    public:

        Draw_Handler(Canvas_Handler& CanvH) : CanvasHandler(CanvH) {}

        std::array<uint8_t, 4>& getActiveColour() { return colour; }
        std::array<uint8_t, 4> getRandomColour() {
            return { static_cast<uint8_t>(rand() % 256),static_cast<uint8_t>(rand() % 256),static_cast<uint8_t>(rand() % 256), 255 };
        }
        void changeColour(const std::array<uint8_t, 4>& varcolour = { 200, 200, 200, 255 }) { colour = varcolour; };

        void drawLine(std::pair<int, int> origin, std::pair<int, int> destination) {

            int x0 = origin.first;
            int y0 = origin.second;
            int x1 = destination.first;
            int y1 = destination.second;

            int dx = std::abs(x1 - x0);
            int dy = -std::abs(y1 - y0);

            int sx = (x0 < x1) ? 1 : -1;
            int sy = (y0 < y1) ? 1 : -1;

            int error = dx + dy;

            while (true) {

                drawPoint({ x0, y0 });

                if (x0 == x1 && y0 == y1)
                    break;

                int e2 = 2 * error;

                if (e2 >= dy) {
                    error += dy;
                    x0 += sx;
                }

                if (e2 <= dx) {
                    error += dx;
                    y0 += sy;
                }
            }
        }

        void processDrawLineCommand() {
            CanvasHandler.CursorHandler.calcNewCursor();
            drawLine(CanvasHandler.CursorHandler.retrieveCursor(), CanvasHandler.CursorHandler.retrieveNewCursor());
            CanvasHandler.CursorHandler.resetCursors();
        }
        void processDrawCircleCommand(bool setting) {
            std::pair<float, float>& c = CanvasHandler.CursorHandler.retrieveCursor();
            int radius = CanvasHandler.CursorHandler.retrieveDrawstep();
            if (setting) {
                for (int i = 1; i <= radius; i++) {
                    colour = getRandomColour();
                    drawCircle(c, i);
                }
                colour = { 200, 200, 200, 255 };
                return;
            }
            else drawCircle(c, radius);
        }

        void processChangeColourCommand(int setting) {
            switch (setting) {
            case 0:
                changeColour();
                break;
            case 1:
                changeColour(getRandomColour());
                break;
            }
        }
        void processRainbowModeCommand(int setting = 100) {
            rainbowMode = !rainbowMode;
            pixelsToRainbow = setting;
        }

    };

#pragma endregion

    Draw_Handler DrawHandler;

    void clearCanvas() {
        for (luxel& l : canvas) {
            l.resetLuxel();
        }
    }

public:

    Canvas_Handler(int& varwidth, int& varheight) :
        width(varwidth), height(varheight),
        CursorHandler(*this),
        DrawHandler(*this),
        canvas(std::vector<luxel>(width* height)) {
       // Only concern here is that if underlying height / width changes then this will need to be recalculated.
    }

    std::vector<luxel>& retrieveCanvas() { return canvas; }
    int& retrieveCanvasWidth() { return width; }
    
    void processDrawCommand(const Command& command) {
        switch (static_cast<Command::DRAW>(command.action)) {
        case Command::DRAW::LINE:
            DrawHandler.processDrawLineCommand();
            break;
        case Command::DRAW::CIRCLE:
            DrawHandler.processDrawCircleCommand(command.setting);
            break;
        }
    };
    void processMetaCommand(const Command& command) {
        switch (static_cast<Command::META>(command.action)) {
        case Command::META::CLEAR:
            clearCanvas();
            break;
        case Command::META::CHANGE_COLOUR:
            DrawHandler.processChangeColourCommand(command.setting);
            break;
        case Command::META::CHANGE_DRAWSTEP:
            CursorHandler.processChangeDrawstepCommand(command.setting);
            break;
        case Command::META::ENABLE_RAINBOW:
            DrawHandler.processRainbowModeCommand(command.setting);
            break;
        }
        

    };
    void processMoveCommand(const Command& command) {
        switch (static_cast<Command::MOVE>(command.action)) {
        case Command::MOVE::UP:
            CursorHandler.processMoveCommand(true, true);
            break;
        case Command::MOVE::RIGHT:
            CursorHandler.processMoveCommand(false, false);
            break;
        case Command::MOVE::DOWN:
            CursorHandler.processMoveCommand(true, false);
            break;
        case Command::MOVE::LEFT:
            CursorHandler.processMoveCommand(false, true);
            break;
        }
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

    int initialiseSDL(int width, int height, const std::vector<luxel>& canvas) {
        if (initialiseWindow(width, height)) return 1;
        if (initialiseRenderer()) return 1;
        if (initialiseTexture(width, height)) return 1;
        SDL_UpdateTexture(Texture, nullptr, canvas.data(), width * sizeof(luxel));
        return 0;
    }

    void refreshPresent(const std::vector<luxel>& canvas, int width) const {
        updateTexture(canvas, width);
        renderTexture();
        renderPresent();
    }

    void cleanup() const {
        if (Texture) SDL_DestroyTexture(Texture);
        if (Renderer) SDL_DestroyRenderer(Renderer);
        if (Window) SDL_DestroyWindow(Window);
        SDL_Quit();
    }

private:

    void updateTexture(const std::vector<luxel>& canvas, int width) const {
        SDL_UpdateTexture(Texture, nullptr, canvas.data(), width * sizeof(luxel));
    }

    void renderTexture() const {
        SDL_RenderTexture(Renderer, Texture, nullptr, nullptr);
    }

    void renderPresent() const {
        SDL_RenderPresent(Renderer);
    }

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

#pragma region Command Handler

struct Command_Handler {

private:

    std::vector<Command> commandQueue;

    // References to handlers needed for command processing
    Canvas_Handler& CanvasHandler;
    SDL_Handler& SDLHandler;

    void processCommand(const Command& command) {
        switch (command.type) {
        case Command::TYPE::DRAW:
            CanvasHandler.processDrawCommand(command);
            break;
        case Command::TYPE::MOVE:
            CanvasHandler.processMoveCommand(command);
            break;
        case Command::TYPE::META:
            CanvasHandler.processMetaCommand(command);
            break;
        }
    }


public:

    void processCommands() {
        for (const auto& command : commandQueue) processCommand(command);
        SDLHandler.refreshPresent(CanvasHandler.retrieveCanvas(), CanvasHandler.retrieveCanvasWidth());
        clearCommands();
    }

    void addCommand(Command command) {
        commandQueue.emplace_back(command);
    }
    void clearCommands() { commandQueue = {}; }

    Command_Handler(Canvas_Handler& varCanvH, SDL_Handler& varSDLH)
        : CanvasHandler(varCanvH), SDLHandler(varSDLH) {}

};
#pragma endregion

#pragma region Keyboard Handler
struct Keyboard_Handler {

private:

    std::unordered_map<SDL_Scancode, Command> keyMapping = {
    { SDL_SCANCODE_W, Command{Command::MOVE::UP} },
    { SDL_SCANCODE_S, Command{Command::MOVE::DOWN} },
    { SDL_SCANCODE_A, Command{Command::MOVE::LEFT} },
    { SDL_SCANCODE_D, Command{Command::MOVE::RIGHT} },

    { SDL_SCANCODE_G, Command{Command::DRAW::CIRCLE} },
    { SDL_SCANCODE_L, Command{Command::DRAW::CIRCLE, 1} },

    { SDL_SCANCODE_J, Command{Command::META::CHANGE_COLOUR, 1} },
    { SDL_SCANCODE_E, Command{Command::META::ENABLE_RAINBOW}},

    { SDL_SCANCODE_Z, Command{Command::META::CHANGE_DRAWSTEP, -2} },
    { SDL_SCANCODE_X, Command{Command::META::CHANGE_DRAWSTEP, 2} },

    { SDL_SCANCODE_C, Command{Command::META::CLEAR} }
    };

public:

    const std::unordered_map<SDL_Scancode, Command>& getKeyboardMapping() { return keyMapping; }
    
};
#pragma endregion

#pragma region Master Handler

struct Master_Handler {

private:

    int width; int height;

    Canvas_Handler CanvasHandler;
    SDL_Handler SDLHandler;
    Command_Handler CommandHandler;
    Keyboard_Handler KeyboardHandler;

    void harvestKeyboardState() {
        const bool* keyboardState = SDL_GetKeyboardState(nullptr);

        for (const auto& [scancode, command] : KeyboardHandler.getKeyboardMapping()) {
            if (keyboardState[scancode]) addCommand(command);
        }
    }

public: 

    Master_Handler(int varwidth, int varheight) : 
        width(varwidth), height(varheight), 
        CanvasHandler(width, height), 
        CommandHandler(CanvasHandler, SDLHandler) {};

    int initialiseSDL() {
        return (SDLHandler.initialiseSDL(width, height, CanvasHandler.retrieveCanvas()));
    }

    void processCommands() {
        harvestKeyboardState();
        CommandHandler.processCommands();
    }
    void addCommand(Command command) {
        CommandHandler.addCommand(command);
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
            if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
                SDL_Scancode keyPressed = event.key.scancode;
                if (keyPressed == SDL_SCANCODE_E) MasterHandler.addCommand(Command{ Command::META::ENABLE_RAINBOW, 100 });
            }
        }

        MasterHandler.processCommands();

    }

    // Clean up
    MasterHandler.cleanup();
    return 0;
}
#pragma endregion