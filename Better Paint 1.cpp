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

    Command(TYPE vartype, int varaction, int varsetting, bool varRepeatable) : type(vartype), action(varaction), setting(varsetting), repeatable(varRepeatable) {}

public:

    struct MOVE {

        enum class ACTION {
            UP = 0,
            RIGHT = 1,
            DOWN = 2,
            LEFT = 3
        };

    private: 

        MOVE(ACTION varAction, int varSetting = 0) : action(varAction), setting(varSetting) {}

    public:

        enum class UP {
            NORMAL = 0
        };
        enum class RIGHT {
            NORMAL = 0
        };
        enum class DOWN {
            NORMAL = 0
        };
        enum class LEFT {
            NORMAL = 0
        };

        ACTION action;
        int setting;

        MOVE(UP varSetting) : MOVE(ACTION::UP, static_cast<int>(varSetting)) {}
        MOVE(RIGHT varSetting) : MOVE(ACTION::RIGHT, static_cast<int>(varSetting)) {}
        MOVE(DOWN varSetting) : MOVE(ACTION::DOWN, static_cast<int>(varSetting)) {}
        MOVE(LEFT varSetting) : MOVE(ACTION::LEFT, static_cast<int>(varSetting)) {}

    };
    struct DRAW {

        enum class ACTION {
            LINE = 0,
            CIRCLE = 1
        };

    private:

        DRAW(ACTION varAction, int varSetting = 0)
            : action(varAction), setting(varSetting) {}

    public:

        enum class LINE {
            NORMAL = 0
        };
        enum class CIRCLE {
            NORMAL = 0,
            RAINBOW = 1
        };

        ACTION action;
        int setting;

        DRAW(LINE varSetting)
            : DRAW(ACTION::LINE, static_cast<int>(varSetting)) {}
        DRAW(CIRCLE varSetting)
            : DRAW(ACTION::CIRCLE, static_cast<int>(varSetting)) {}
    };
    struct META {

        enum class ACTION {
            CLEAR = 0,
            CHANGE_COLOUR = 1,
            CHANGE_DRAWSTEP = 2,
            ENABLE_RAINBOW = 3
        };

    private:

        META(ACTION varAction, int varSetting = 0)
            : action(varAction), setting(varSetting) {}

    public:

        enum class CLEAR {
            NORMAL = 0
        };
        enum class CHANGE_COLOUR {
            DEFAULT = 0,
            RANDOM = 1
        };
        enum class CHANGE_DRAWSTEP {
            INCREASE = 0,
            DECREASE = 1
        };
        enum class ENABLE_RAINBOW {
            NORMAL = 0
        };

        ACTION action;
        int setting;

        META(CLEAR varSetting)
            : META(ACTION::CLEAR, static_cast<int>(varSetting)) {}
        META(CHANGE_COLOUR varSetting)
            : META(ACTION::CHANGE_COLOUR, static_cast<int>(varSetting)) {}
        META(CHANGE_DRAWSTEP varSetting)
            : META(ACTION::CHANGE_DRAWSTEP, static_cast<int>(varSetting)) {}
        META(ENABLE_RAINBOW varSetting)
            : META(ACTION::ENABLE_RAINBOW, static_cast<int>(varSetting)) {}
    };


    Command(MOVE cmd, bool repeatable) : Command(TYPE::MOVE, static_cast<int>(cmd.action), static_cast<int>(cmd.setting), repeatable) {}
    Command(DRAW cmd, bool repeatable) : Command(TYPE::DRAW, static_cast<int>(cmd.action), static_cast<int>(cmd.setting), repeatable) {}
    Command(META cmd, bool repeatable) : Command(TYPE::META, static_cast<int>(cmd.action), static_cast<int>(cmd.setting), repeatable) {}

    TYPE type;
    int action; // differentiates between commands.
    int setting = 0; // for passing additional information ?
    bool repeatable = true;

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

#pragma region Cursor Handler

    struct Cursor_Handler {

    private:

        Canvas_Handler& CanvasHandler;

        std::pair<float, float> cursor = { 100, 100 };
        std::pair<float, float> newCursor = { 100, 100 };
        std::pair<float, float> deltaCursor = { 0, 0 }; // newCursor = cursor + deltaCursor at t==0, cursor = newCursor at t==1;
        

        int drawStep = 2;
        int drawStepDelta = 2;

        void adjustDeltaCursor(std::pair<float, float> incoming) { deltaCursor.first += incoming.first; deltaCursor.second += incoming.second; }
      
        void updateDrawstep(int delta) {
            drawStep += delta;
            if (drawStep < 2) drawStep = 2;
            if (drawStep > 50) drawStep = 50;
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

        void processChangeDrawstepCommand(const Command::META::CHANGE_DRAWSTEP& setting) {
            int tDSDelta = drawStepDelta * (setting == Command::META::CHANGE_DRAWSTEP::DECREASE ? -1 : 1); // setting == 0 if increase, == 1 if decrease;
            updateDrawstep(tDSDelta);
        }
        void processMoveCommand(bool yaxis, bool negative, int setting = 0) {
            updateDeltaCursor(yaxis, negative);
            calcNewCursor();
            CanvasHandler.DrawHandler.drawLine(cursor, newCursor);
            resetCursors();
        }

        size_t pixelsDrawn = 0;

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

        void updateDrawData() {
            CanvasHandler.CursorHandler.pixelsDrawn += 1;
            if (rainbowMode) if (CanvasHandler.CursorHandler.pixelsDrawn > pixelsToRainbow) {
                colour = getRandomColour();
                CanvasHandler.CursorHandler.pixelsDrawn = 0;
            }
        }

        void drawPoint(const std::pair<float, float>& c) {
            luxel* ptr = CanvasHandler.retrieveLuxelFromPoint(c);
            if (!ptr) return;
            ptr->colour = colour;
            updateDrawData();
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

        bool rainbowMode = false; int pixelsToRainbow = 100;



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

        void processDrawLineCommand(const Command::DRAW::LINE& setting) {
            // fizzbuzz
        }
        void processDrawCircleCommand(const Command::DRAW::CIRCLE& setting) {
            std::pair<float, float>& c = CanvasHandler.CursorHandler.retrieveCursor();
            int radius = CanvasHandler.CursorHandler.retrieveDrawstep();
            if (setting == Command::DRAW::CIRCLE::RAINBOW) {
                for (int i = 1; i <= radius; i++) {
                    colour = getRandomColour();
                    drawCircle(c, i);
                }
                colour = { 200, 200, 200, 255 };
                return;
            }
            else drawCircle(c, radius);
        }

        void processChangeColourCommand(const Command::META::CHANGE_COLOUR& setting) {
            switch (setting) {
            case Command::META::CHANGE_COLOUR::DEFAULT:
                changeColour();
                break;
            case Command::META::CHANGE_COLOUR::RANDOM:
                changeColour(getRandomColour());
                break;
            }
        }
        void processRainbowModeCommand(const Command::META::ENABLE_RAINBOW& setting) {
            rainbowMode = !rainbowMode;
        }

    };

#pragma endregion

    Draw_Handler DrawHandler;

    void clearCanvas() {
        for (luxel& l : canvas) {
            l.resetLuxel();
        }
    }
    bool coordCheck(const std::pair<float, float>& c) {
        if (c.first < 0 or c.first > width) return false;
        if (c.second < 0 or c.second >= height) return false;
        return true;
    }
    luxel* retrieveLuxelFromIndex(const size_t& index) { return &canvas[index]; }
    luxel* retrieveLuxelFromPoint(const std::pair<float, float>& c, bool coordCheck) { return &canvas[indexFromCoord(c)]; }

public:

    Canvas_Handler(int& varwidth, int& varheight) :
        width(varwidth), height(varheight),
        CursorHandler(*this),
        DrawHandler(*this),
        canvas(std::vector<luxel>(width* height)) {
       // Only concern here is that if underlying height / width changes then this will need to be recalculated.
    }

    std::vector<luxel>& retrieveCanvas() { return canvas; }

    luxel* retrieveLuxelFromPoint(const std::pair<float, float>& c) { return (coordCheck(c) ? retrieveLuxelFromPoint(c, true) : nullptr); }

    int& retrieveCanvasWidth() { return width; }
    
    void processDrawCommand(const Command& command) {
        switch (static_cast<Command::DRAW::ACTION>(command.action)) {
        case Command::DRAW::ACTION::LINE:
            DrawHandler.processDrawLineCommand(static_cast<Command::DRAW::LINE>(command.setting));
            break;
        case Command::DRAW::ACTION::CIRCLE:
            DrawHandler.processDrawCircleCommand(static_cast<Command::DRAW::CIRCLE>(command.setting));
            break;
        }
    };
    void processMetaCommand(const Command& command) {
        switch (static_cast<Command::META::ACTION>(command.action)) {
        case Command::META::ACTION::CLEAR:
            clearCanvas();
            break;
        case Command::META::ACTION::CHANGE_COLOUR:
            DrawHandler.processChangeColourCommand(static_cast<Command::META::CHANGE_COLOUR>(command.setting));
            break;
        case Command::META::ACTION::CHANGE_DRAWSTEP:
            CursorHandler.processChangeDrawstepCommand(static_cast<Command::META::CHANGE_DRAWSTEP>(command.setting));
            break;
        case Command::META::ACTION::ENABLE_RAINBOW:
            DrawHandler.processRainbowModeCommand(static_cast<Command::META::ENABLE_RAINBOW>(command.setting));
            break;
        }
        

    };
    void processMoveCommand(const Command& command) {
        switch (static_cast<Command::MOVE::ACTION>(command.action)) {
        case Command::MOVE::ACTION::UP:
            CursorHandler.processMoveCommand(true, true);
            break;
        case  Command::MOVE::ACTION::RIGHT:
            CursorHandler.processMoveCommand(false, false);
            break;
        case  Command::MOVE::ACTION::DOWN:
            CursorHandler.processMoveCommand(true, false);
            break;
        case  Command::MOVE::ACTION::LEFT:
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
    { SDL_SCANCODE_W, Command{Command::MOVE::UP::NORMAL, true} },
    { SDL_SCANCODE_S, Command{Command::MOVE::DOWN::NORMAL, true} },
    { SDL_SCANCODE_A, Command{Command::MOVE::LEFT::NORMAL, true} },
    { SDL_SCANCODE_D, Command{Command::MOVE::RIGHT::NORMAL, true} },

    { SDL_SCANCODE_G, Command{Command::DRAW::CIRCLE::NORMAL, false} },
    { SDL_SCANCODE_L, Command{Command::DRAW::CIRCLE::RAINBOW, false} },

    { SDL_SCANCODE_J, Command{Command::META::CHANGE_COLOUR::RANDOM, false} },
    { SDL_SCANCODE_E, Command{Command::META::ENABLE_RAINBOW::NORMAL, false} },

    { SDL_SCANCODE_Z, Command{Command::META::CHANGE_DRAWSTEP::DECREASE, false} },
    { SDL_SCANCODE_X, Command{Command::META::CHANGE_DRAWSTEP::INCREASE, false} },

    { SDL_SCANCODE_C, Command{Command::META::CLEAR::NORMAL, false} }
    };

    Command_Handler& CommandHandler;
    std::array<bool, SDL_SCANCODE_COUNT> previousKeyboardState{};

public:

    void harvestKeyboardState() {
        const bool* keyboardState = SDL_GetKeyboardState(nullptr);
        for (const auto& [scancode, command] : getKeyboardMapping()) {
            
            if (keyboardState[scancode] &&
                (command.repeatable || !previousKeyboardState[scancode])) CommandHandler.addCommand(command);

            previousKeyboardState[scancode] = keyboardState[scancode];
        } 
    }
    const std::unordered_map<SDL_Scancode, Command>& getKeyboardMapping() { return keyMapping; }

    Keyboard_Handler(Command_Handler& CommH) : CommandHandler(CommH) {}
    
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



public: 

    Master_Handler(int varwidth, int varheight) : 
        width(varwidth), height(varheight), 
        CanvasHandler(width, height), 
        CommandHandler(CanvasHandler, SDLHandler),
        KeyboardHandler(CommandHandler) {};

    int initialiseSDL() {
        return (SDLHandler.initialiseSDL(width, height, CanvasHandler.retrieveCanvas()));
    }

    void processCommands() {
        KeyboardHandler.harvestKeyboardState();
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
        }

        MasterHandler.processCommands();

    }

    // Clean up
    MasterHandler.cleanup();
    return 0;
}
#pragma endregion