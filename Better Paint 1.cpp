// Better Paint 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <vector>
#include <array>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <variant>
/*
TO DO:

1. Separate program into files.
2. Brush modes
3. Add visual cursor


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
        META = 2,
        APP = 3
    };

    using Payload = std::variant<std::monostate, int, std::pair<float, float>, std::array<uint8_t, 4>>;

private:

    Command(TYPE vartype, int varaction, int varsetting, bool varRepeatable, Payload varPayload = {}) :
        type(vartype), action(varaction), setting(varsetting), repeatable(varRepeatable), payload(varPayload) {}

public:

#pragma region MOVE

    struct MOVE {

        enum class ACTION {
            DIRECTION = 0,
            SET = 1
        };

    private:

        MOVE(ACTION varAction, int varSetting = 0) : action(varAction), setting(varSetting) {}

    public:

        enum class DIRECTION {
            RIGHT = 0,
            DOWN = 1,
            LEFT = 2,
            UP = 3
        };
        enum class SET {
            RESET_TO_ORIGIN = 0,
            USE_PAYLOAD = 1
        };

        ACTION action;
        int setting;

        MOVE(DIRECTION varSetting) : MOVE(ACTION::DIRECTION, static_cast<int>(varSetting)) {}
        MOVE(SET varSetting) : MOVE(ACTION::SET, static_cast<int>(varSetting)) {}

    };

#pragma endregion

#pragma region DRAW

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

#pragma endregion

#pragma region META

    struct META {

        enum class ACTION {
            CLEAR = 0,
            CHANGE_COLOUR = 1,
            CHANGE_DRAWSTEP = 2,
            ENABLE_RAINBOW = 3,
            CHANGE_PEN_WIDTH = 4,
            PEN_DOWN = 5,
            SAVE_ORIGIN = 6
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
            USE_PAYLOAD = 1,
            RANDOM = 2
        };
        enum class CHANGE_DRAWSTEP {
            USE_PAYLOAD = 0,
            SET_TO_PAYLOAD = 1
        };
        enum class ENABLE_RAINBOW {
            DEFAULT = 0,
            USE_PAYLOAD = 1
        };
        enum class CHANGE_PEN_WIDTH {
            USE_PAYLOAD = 0
        };
        enum class PEN_DOWN {
            NORMAL = 0
        };
        enum class SAVE_ORIGIN {
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
        META(CHANGE_PEN_WIDTH varSetting)
            : META(ACTION::CHANGE_PEN_WIDTH, static_cast<int>(varSetting)) {}
        META(PEN_DOWN varSetting)
            : META(ACTION::PEN_DOWN, static_cast<int>(varSetting)) {}
        META(SAVE_ORIGIN varSetting)
            : META(ACTION::SAVE_ORIGIN, static_cast<int>(varSetting)) {}
    };

#pragma endregion

#pragma region APP
    struct APP {
    
        enum class ACTION {
            CLI = 0
        };
    
    private:

        APP(ACTION varAction, int varSetting = 0) : action(varAction), setting(varSetting) {}

    public:

        enum class CLI {
            OPEN = 0
        };

        APP(CLI varSetting) : APP(ACTION::CLI, static_cast<int>(varSetting)) {}

        ACTION action;
        int setting;

    };
#pragma endregion

    Command(MOVE cmd, bool repeatable, Payload payload = {}) : Command(TYPE::MOVE, static_cast<int>(cmd.action), static_cast<int>(cmd.setting), repeatable, payload) {}
    Command(DRAW cmd, bool repeatable, Payload payload = {}) : Command(TYPE::DRAW, static_cast<int>(cmd.action), static_cast<int>(cmd.setting), repeatable, payload) {}
    Command(META cmd, bool repeatable, Payload payload = {}) : Command(TYPE::META, static_cast<int>(cmd.action), static_cast<int>(cmd.setting), repeatable, payload) {}
    Command(APP cmd,  bool repeatable, Payload payload = {}) : Command(TYPE::APP,  static_cast<int>(cmd.action), static_cast<int>(cmd.setting), repeatable, payload) {}

    TYPE type;
    int action; // differentiates between commands.
    int setting; // settings within commands.
    bool repeatable = true;
    Payload payload;

};

enum class COMMAND {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_RESET,
    MOVE_SET_POINT,
    MOVE_SAVE_ORIGIN,

    DRAW_CIRCLE,
    DRAW_CIRCLE_RAINBOW,

    COLOUR_RESET,
    COLOUR_RANDOM,
    COLOUR_SET,

    RAINBOW_DEFAULT,
    RAINBOW_SET,

    DRAWSTEP_DECREASE,
    DRAWSTEP_INCREASE,
    DRAWSTEP_SET,

    PEN_DOWN,
    PEN_WIDTH_DECREASE,
    PEN_WIDTH_INCREASE,

    CLI_OPEN,

    CLEAR,

    INVALID
};

inline static const std::unordered_map<COMMAND, Command> commandMapping = {
    // ANY CONSTANTS DEFINED HERE SHOULD USE constructCommand() TO PASS INFORMATION
    // CONSTANTS DEFINED HERE ARE SET FOR COMMANDS THAT MUST HAVE PAYLOAD INFORMATION TO RUN.
    { COMMAND::MOVE_UP,             Command{Command::MOVE::DIRECTION::UP, true} },
    { COMMAND::MOVE_DOWN,           Command{Command::MOVE::DIRECTION::DOWN, true} },
    { COMMAND::MOVE_LEFT,           Command{Command::MOVE::DIRECTION::LEFT, true} },
    { COMMAND::MOVE_RIGHT,          Command{Command::MOVE::DIRECTION::RIGHT, true} },
    { COMMAND::MOVE_RESET,          Command{Command::MOVE::SET::RESET_TO_ORIGIN, false} },
    { COMMAND::MOVE_SET_POINT,      Command{Command::MOVE::SET::USE_PAYLOAD, false, std::pair<float,float>{ 100.0f, 100.0f }} },

    { COMMAND::DRAW_CIRCLE,         Command{Command::DRAW::CIRCLE::NORMAL, false} },
    { COMMAND::DRAW_CIRCLE_RAINBOW, Command{Command::DRAW::CIRCLE::RAINBOW, false} },

    { COMMAND::COLOUR_RESET,        Command{Command::META::CHANGE_COLOUR::DEFAULT, false} },
    { COMMAND::COLOUR_RANDOM,       Command{Command::META::CHANGE_COLOUR::RANDOM, false} },
    { COMMAND::COLOUR_SET,          Command{Command::META::CHANGE_COLOUR::USE_PAYLOAD, false, std::array<uint8_t, 4>{200, 200, 200, 255}} },

    { COMMAND::RAINBOW_DEFAULT,     Command{Command::META::ENABLE_RAINBOW::DEFAULT, false} },
    { COMMAND::RAINBOW_SET,         Command{Command::META::ENABLE_RAINBOW::USE_PAYLOAD, false, 1000} },

    { COMMAND::DRAWSTEP_DECREASE,   Command{Command::META::CHANGE_DRAWSTEP::USE_PAYLOAD, false, -1} },
    { COMMAND::DRAWSTEP_INCREASE,   Command{Command::META::CHANGE_DRAWSTEP::USE_PAYLOAD, false,  1} },
    { COMMAND::DRAWSTEP_SET,        Command{Command::META::CHANGE_DRAWSTEP::SET_TO_PAYLOAD, false,  1} },


    { COMMAND::PEN_DOWN,            Command{Command::META::PEN_DOWN::NORMAL, false} },
    { COMMAND::PEN_WIDTH_DECREASE,  Command{Command::META::CHANGE_PEN_WIDTH::USE_PAYLOAD, false, -1} },
    { COMMAND::PEN_WIDTH_INCREASE,  Command{Command::META::CHANGE_PEN_WIDTH::USE_PAYLOAD, false, 1} },

    { COMMAND::CLEAR,               Command{Command::META::CLEAR::NORMAL, false} },
    { COMMAND::MOVE_SAVE_ORIGIN,    Command{Command::META::SAVE_ORIGIN::NORMAL, false} },

    { COMMAND::CLI_OPEN,            Command{Command::APP::CLI::OPEN, false} },

};

#pragma endregion

#pragma region Canvas Handler
struct Canvas_Handler {

private: 

#pragma region Cursor Handler
    struct Cursor_Handler {

        Cursor_Handler(Canvas_Handler& varCanvH) : CanvasHandler(varCanvH) {}
        Canvas_Handler& CanvasHandler;

        std::pair<float, float> origin = { 100, 100 };
        std::pair<float, float> cursor = { 100, 100 };
        std::pair<float, float> deltaCursor = { 0, 0 };
        
        int drawStep = 1, drawStepDelta = 1;
        bool penDown = true;
        size_t pixelsDrawn = 0;

        static void resetPoint(float& f, const int& width) {
            f = std::clamp(f, 0.0f, static_cast<float>(width - 1));
        }
        void resetCursors() {
            cursor = CanvasHandler.addCoords(cursor, deltaCursor);
            deltaCursor = { 0 , 0 };
            if (!CanvasHandler.coordCheck(cursor)) {
                resetPoint(cursor.first, CanvasHandler.width); resetPoint(cursor.second, CanvasHandler.width);
            }
        }

        void checkCursorLine() {
            if (penDown) CanvasHandler.DrawHandler.drawLine(cursor, CanvasHandler.addCoords(cursor, deltaCursor), true);
            resetCursors();
        }

        void processChangeDrawstepCommand(const Command& command) {
            using setting = Command::META::CHANGE_DRAWSTEP;
            switch (static_cast<setting>(command.setting)) {
            case setting::USE_PAYLOAD:
                drawStep = std::clamp(drawStep + std::get<int>(command.payload), 1, 50);
                break;
            case setting::SET_TO_PAYLOAD:
                drawStep = std::clamp(std::get<int>(command.payload), 1, 50);
                break;
            }
            
        } 
        void processDirectionCommand(const Command& command) {
            switch (static_cast<Command::MOVE::DIRECTION>(command.setting)) {
            case Command::MOVE::DIRECTION::UP:
                deltaCursor.second -= drawStep;
                break;
            case  Command::MOVE::DIRECTION::RIGHT:
                deltaCursor.first += drawStep;
                break;
            case  Command::MOVE::DIRECTION::DOWN:
                deltaCursor.second += drawStep;
                break;
            case  Command::MOVE::DIRECTION::LEFT:
                deltaCursor.first -= drawStep;
                break;
            }
        }
        void processSetCommand(const Command& command) {
            switch (static_cast<Command::MOVE::SET>(command.setting)) {
            case Command::MOVE::SET::RESET_TO_ORIGIN:
                cursor = origin;
                break;
            case Command::MOVE::SET::USE_PAYLOAD:
                cursor = std::get<std::pair<float, float>>(command.payload);
                break;
            }
        }
        void processMoveCommand(const Command& command) {
            switch (static_cast<Command::MOVE::ACTION>(command.action)) {
            case Command::MOVE::ACTION::DIRECTION:
                processDirectionCommand(command);
                break;
            case Command::MOVE::ACTION::SET:
                processSetCommand(command);
                break;
            }
        }

        
    };
#pragma endregion
#pragma region Draw Handler
    struct Draw_Handler {

        Canvas_Handler& CanvasHandler;
        std::array<uint8_t, 4> colour = { 200, 200, 200, 255 };
        bool rainbowMode = false; int pixelsToRainbow = 1000;
        int pen = 1, penDelta = 1;

        void checkDrawData() {
            if (rainbowMode) if (CanvasHandler.CursorHandler.pixelsDrawn > pixelsToRainbow) {
                colour = getRandomColour();
                CanvasHandler.CursorHandler.pixelsDrawn = 0;
            }
        }

        void drawPoint(const std::pair<float, float>& c) {
            luxel* ptr = CanvasHandler.retrieveLuxelFromPoint(c);
            if (!ptr) return;
            ptr->colour = colour;
            CanvasHandler.CursorHandler.pixelsDrawn += 1;
        }

        void drawLine(std::pair<int, int> origin, std::pair<int, int> destination, const bool useP) {

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

                drawPoint({ x0, y0 }, useP);

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

        void drawCircle(const std::pair<float, float>& c, int radius, const bool fill = false) {
            int x = 0;
            int y = radius;
            int d = 1 - radius;

            while (x <= y) {

                if (fill) {
                    drawLine({ c.first - x, c.second + y }, { c.first + x, c.second + y }, false);
                    drawLine({ c.first - x, c.second - y }, { c.first + x, c.second - y }, false);

                    drawLine({ c.first - y, c.second + x }, { c.first + y, c.second + x }, false);
                    drawLine({ c.first - y, c.second - x }, { c.first + y, c.second - x }, false);
                }
                else {
                    drawPoint({ c.first + x, c.second + y });
                    drawPoint({ c.first - x, c.second + y });
                    drawPoint({ c.first + x, c.second - y });
                    drawPoint({ c.first - x, c.second - y });

                    drawPoint({ c.first + y, c.second + x });
                    drawPoint({ c.first - y, c.second + x });
                    drawPoint({ c.first + y, c.second - x });
                    drawPoint({ c.first - y, c.second - x });
                }

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
        
        void usePen(const std::pair<float, float>& c) {
            drawCircle(c, pen, true);
            checkDrawData();
        }
        void drawPoint(const std::pair<float, float>& c, const bool useP) {
            if (useP) usePen(c);
            else drawPoint(c);
        }


        Draw_Handler(Canvas_Handler& CanvH) : CanvasHandler(CanvH) {}

        std::array<uint8_t, 4>& getActiveColour() { return colour; }
        static std::array<uint8_t, 4> getRandomColour() {
            return { static_cast<uint8_t>(rand() % 256),static_cast<uint8_t>(rand() % 256),static_cast<uint8_t>(rand() % 256), 255 };
        }
        void changeColour(const std::array<uint8_t, 4>& varcolour = { 200, 200, 200, 255 }) { colour = varcolour; };

        void processDrawLineCommand(const Command::DRAW::LINE& setting) {
            // fizzbuzz
        }
        void processDrawCircleCommand(const Command& command) {
            auto setting = static_cast<Command::DRAW::CIRCLE>(command.setting);
            
            int radius = std::holds_alternative<int>(command.payload)
                ? std::get<int>(command.payload)
                : CanvasHandler.CursorHandler.drawStep;

            if (setting == Command::DRAW::CIRCLE::RAINBOW) {
                for (int i = 1; i <= radius; i++) {
                    colour = getRandomColour();
                    drawCircle(CanvasHandler.CursorHandler.cursor, i);
                }
                colour = { 200, 200, 200, 255 };
                return;
            }
            else drawCircle(CanvasHandler.CursorHandler.cursor, radius);
        }

        void processChangeColourCommand(const Command& command) {
            switch (static_cast<Command::META::CHANGE_COLOUR>(command.setting)) {
            case Command::META::CHANGE_COLOUR::DEFAULT:
                changeColour();
                break;
            case Command::META::CHANGE_COLOUR::USE_PAYLOAD:
                changeColour(std::get<std::array<uint8_t, 4>>(command.payload));
                break;
            case Command::META::CHANGE_COLOUR::RANDOM:
                changeColour(getRandomColour());
                break;
            }
        }
        void processRainbowModeCommand(const Command& command) {
            using setting = Command::META::ENABLE_RAINBOW;
            switch (static_cast<setting>(command.setting)) {
            case setting::USE_PAYLOAD:
                {
                    int candidate = std::get<int>(command.payload);
                    if (candidate != pixelsToRainbow) pixelsToRainbow = candidate;
                    else rainbowMode = !rainbowMode;
                    break; // scoping candidate to allow us to declare in this case branch without affecting other
                }
            case setting::DEFAULT:
                rainbowMode = !rainbowMode;
                break;
            }
        }
        void processChangePenWidthCommand(const Command& command) {
            pen = std::clamp(pen + std::get<int>(command.payload), 1, 50);
        }

    };
#pragma endregion

    Cursor_Handler CursorHandler;
    Draw_Handler DrawHandler;

    void clearCanvas() {
        for (luxel& l : canvas) {
            l.resetLuxel();
        }
    }

    static inline size_t indexFromCoord(const std::pair<float, float>& c, const int& w) {
        // ASSUMES POSITIVE X/Y. INDEXING WITH THIS INDEX WITHOUT SIZE CHECKING MAY CAUSE OUT OF BOUNDARY MEMORY CRASH [IF COORD > LAST LUXEL INDEX].
        return size_t(c.second) * w + size_t(c.first);
    }
    size_t indexFromCoord(const std::pair<float, float>& c) const {
        return indexFromCoord(c, width);
    }

    bool coordCheck(const std::pair<float, float>& c) const  {
        if (c.first < 0 or c.first > width) return false;
        if (c.second < 0 or c.second >= height) return false;
        return true;
    }
    static inline std::pair<float, float> addCoords(const std::pair<float, float>& c1, const std::pair<float, float>& c2) {
        return { c1.first + c2.first, c1.second + c2.second };
    }

    luxel* retrieveLuxelFromIndex(const size_t& index) { return &canvas[index]; }
    luxel* retrieveLuxelFromPoint(const std::pair<float, float>& c, bool coordCheck) { return &canvas[indexFromCoord(c)]; }
    luxel* retrieveLuxelFromPoint(const std::pair<float, float>& c) { return (coordCheck(c) ? retrieveLuxelFromPoint(c, true) : nullptr); }


public:

    Canvas_Handler(int& varwidth, int& varheight) :
        width(varwidth), height(varheight),
        CursorHandler(*this),
        DrawHandler(*this),
        canvas(std::vector<luxel>(width* height)) {
       // Only concern here is that if underlying height / width changes then this will need to be recalculated.
    }
    
    int& width; int& height;
    std::vector<luxel> canvas;

    void refreshCursor() { CursorHandler.checkCursorLine(); }
    
    void processDrawCommand(const Command& command) {
        switch (static_cast<Command::DRAW::ACTION>(command.action)) {
        case Command::DRAW::ACTION::LINE:
            DrawHandler.processDrawLineCommand(static_cast<Command::DRAW::LINE>(command.setting));
            break;
        case Command::DRAW::ACTION::CIRCLE:
            DrawHandler.processDrawCircleCommand(command);
            break;
        }
    };
    void processMetaCommand(const Command& command) {
        switch (static_cast<Command::META::ACTION>(command.action)) {
        case Command::META::ACTION::CLEAR:
            clearCanvas();
            break;
        case Command::META::ACTION::CHANGE_COLOUR:
            DrawHandler.processChangeColourCommand(command);
            break;
        case Command::META::ACTION::CHANGE_DRAWSTEP:
            CursorHandler.processChangeDrawstepCommand(command);
            break;
        case Command::META::ACTION::ENABLE_RAINBOW:
            DrawHandler.processRainbowModeCommand(command);
            break;
        case Command::META::ACTION::CHANGE_PEN_WIDTH:
            DrawHandler.processChangePenWidthCommand(command);
            break;
        case Command::META::ACTION::PEN_DOWN:
            CursorHandler.penDown = !CursorHandler.penDown;
            break;
        case Command::META::ACTION::SAVE_ORIGIN:
            CursorHandler.origin = CursorHandler.cursor;
            break;
        }
    };
    void processMoveCommand(const Command& command) {
        CursorHandler.processMoveCommand(command);
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

    Master_Handler& MasterHandler;
    std::vector<Command> commandQueue;

    void processCommand(const Command& command);

public:

    void processCommands();
    
    int commandQueueSize() { return commandQueue.size(); }
    void addCommand(Command command) {
        commandQueue.emplace_back(std::move(command));
    }
    void constructCommand(COMMAND command, Command::Payload payload = {}) {
        Command result = commandMapping.at(command);
        result.payload = payload;
        addCommand(result);
    }

    void clearCommands() { commandQueue = {}; }

    Command_Handler(Master_Handler& mastH)
        : MasterHandler(mastH) {}

};

#pragma endregion

#pragma region Keyboard Handler
struct Keyboard_Handler {

private:

    std::unordered_map<SDL_Scancode, COMMAND> keyMapping = {
        { SDL_SCANCODE_W, COMMAND::MOVE_UP },
        { SDL_SCANCODE_S, COMMAND::MOVE_DOWN },
        { SDL_SCANCODE_A, COMMAND::MOVE_LEFT },
        { SDL_SCANCODE_D, COMMAND::MOVE_RIGHT },
        { SDL_SCANCODE_Q, COMMAND::MOVE_RESET },

        { SDL_SCANCODE_1, COMMAND::PEN_DOWN },

        { SDL_SCANCODE_G, COMMAND::DRAW_CIRCLE },
        { SDL_SCANCODE_L, COMMAND::DRAW_CIRCLE_RAINBOW },

        { SDL_SCANCODE_J, COMMAND::COLOUR_RANDOM },
        { SDL_SCANCODE_E, COMMAND::RAINBOW_DEFAULT },

        { SDL_SCANCODE_Z, COMMAND::DRAWSTEP_DECREASE },
        { SDL_SCANCODE_X, COMMAND::DRAWSTEP_INCREASE },

        { SDL_SCANCODE_V, COMMAND::PEN_WIDTH_DECREASE },
        { SDL_SCANCODE_B, COMMAND::PEN_WIDTH_INCREASE },

        { SDL_SCANCODE_C, COMMAND::CLEAR },
        { SDL_SCANCODE_2, COMMAND::MOVE_SAVE_ORIGIN },
        { SDL_SCANCODE_0, COMMAND::CLI_OPEN }
    };

    Command_Handler& CommandHandler;
    std::array<bool, SDL_SCANCODE_COUNT> previousKeyboardState{};

public:

    void harvestKeyboardState() {
        const bool* keyboardState = SDL_GetKeyboardState(nullptr);
        for (const auto& [scancode, binding] : keyMapping) {
            Command comm = commandMapping.at(binding);
            if (keyboardState[scancode] &&
                (comm.repeatable || !previousKeyboardState[scancode])) CommandHandler.addCommand(comm); // NOT USING CONSTRUCTCOMMAND AS KEYBOARD INPUTS CARRY NO ADDITIONAL DATA

            previousKeyboardState[scancode] = keyboardState[scancode];
        }
    }

    Keyboard_Handler(Command_Handler& CommH) : CommandHandler(CommH) {}

};
#pragma endregion

#pragma region CLI Handler
struct CLI_Handler {
private:

    Master_Handler& MasterHandler;

    inline static const std::unordered_map<std::string, COMMAND> stringMapping = {
    { "up",              COMMAND::MOVE_UP },
    { "down",            COMMAND::MOVE_DOWN },
    { "left",            COMMAND::MOVE_LEFT },
    { "right",           COMMAND::MOVE_RIGHT },
    { "reset",           COMMAND::MOVE_RESET },
    { "move",            COMMAND::MOVE_SET_POINT },
    { "save_origin",     COMMAND::MOVE_SAVE_ORIGIN },

    { "circle",          COMMAND::DRAW_CIRCLE },
    { "rainbow_circle",  COMMAND::DRAW_CIRCLE_RAINBOW },

    { "default_colour",  COMMAND::COLOUR_RESET },
    { "random_colour",   COMMAND::COLOUR_RANDOM },
    { "colour",          COMMAND::COLOUR_SET },

    { "rainbow",         COMMAND::RAINBOW_SET },

    { "step_down",       COMMAND::DRAWSTEP_DECREASE },
    { "step_up",         COMMAND::DRAWSTEP_INCREASE },
    { "step",            COMMAND::DRAWSTEP_SET },

    { "pen",             COMMAND::PEN_DOWN },
    { "pen_thinner",     COMMAND::PEN_WIDTH_DECREASE },
    { "pen_thicker",     COMMAND::PEN_WIDTH_INCREASE },

    { "clear",           COMMAND::CLEAR }
    };

    std::string harvestInput(const std::string& Question, bool linebreak = true) {
        std::cout << Question << std::endl;
        std::string output;
        std::cin >> output;
        if (linebreak) std::cout << std::endl;
        return output;
    }

    std::pair<float, float> convertCoord(const std::string& input) {
        std::pair<float, float> ret;
        std::string s = "";
        for (char c : input) {
            switch (c) {
            case '{':
                continue;
                break;
            case ',':
                ret.first = (std::stof(s));
                s = "";
                break;
            case '}':
                ret.second = (std::stof(s));
                break;
            default:
                s += c;
            }
        }
        return ret;
    }
    std::array<uint8_t, 4> convertColour(const std::string& input) {
        std::array<uint8_t, 4> ret = { 200, 200, 200, 255 };
        int i = 0;
        std::string s = "";
        for (char c : input) {
            switch (c) {
            case '{':
                continue;
                break;
            case '}':
                [[fallthrough]];
            case ',':
                ret[i] = static_cast<uint8_t>(std::stoi(s));
                s = "";
                i++;
                break;
            default:
                s += c;
            }
        }
        return ret;
    }

    using Payload = std::variant<std::monostate, int, std::pair<float, float>, std::array<uint8_t, 4>>;

    Payload harvestPayload() {
        std::string input = harvestInput(R"(
Please input data for payload: 

    Integers should be input as such: 2
    Coordinates should be input as such: {100,100}
    Colours should be input as such: {200,200,200,255}")");

        switch (std::stoi(harvestInput(R"(
Please enter the payload's type:

    Integer:    [1]
    Coordinate: [2]
    Colour:     [3]

Or enter anything else to cancel)")))
        {
        case 1:
            return std::stoi(input);
        case 2:
            return convertCoord(input);
        case 3:
            return convertColour(input);
        default:
            return {};

        }

    }

    void createCommand();
    void CLI_Loop();

public:

    void beginCLILoop();

    CLI_Handler(Master_Handler& mastH) : MasterHandler(mastH) {}

};
#pragma endregion

#pragma region Master Handler

struct Master_Handler {

private:

    int width; int height;

public: 

    Canvas_Handler CanvasHandler;
    SDL_Handler SDLHandler;
    Command_Handler CommandHandler;
    Keyboard_Handler KeyboardHandler;
    CLI_Handler CLIHandler;

    Master_Handler(int varwidth, int varheight) : 
        width(varwidth), height(varheight), 
        CanvasHandler(width, height), 
        CommandHandler(*this),
        KeyboardHandler(CommandHandler),
        CLIHandler(*this){};

    int initialiseSDL() {
        return (SDLHandler.initialiseSDL(width, height, CanvasHandler.canvas));
    }

    void processCommands() {
        KeyboardHandler.harvestKeyboardState();
        CommandHandler.processCommands();
    }

    void cleanup() const {
        SDLHandler.cleanup();
    }

};

#pragma endregion

#pragma region DEPENDENT FUNCTIONS

#pragma region CLI_Handler
void CLI_Handler::createCommand() {

    // NO INPUT CHECKS.  WE DIE LIKE REAL MEN.

    COMMAND command = COMMAND::INVALID;

    std::string commandName = harvestInput("Please enter the name of the command: ");
    if (stringMapping.contains(commandName)) command = stringMapping.at(commandName);
    else return;

    if (!(harvestInput("Add Payload? [Y]") == "Y")) MasterHandler.CommandHandler.constructCommand(command);
    else MasterHandler.CommandHandler.constructCommand(command, harvestPayload());

};
void CLI_Handler::CLI_Loop() {
    while (true) {
        std::cout << R"(
You are at the beginning of the CLI loop.

Options:

    [1] Add command.
    [2] Execute current commands.
    [3] Quit.

)" << std::endl;
        switch (std::stoi(harvestInput(""))) {
        case 1:
            createCommand();
            break;
        case 2:
            MasterHandler.CommandHandler.processCommands();
            break;
        case 3:
            if (MasterHandler.CommandHandler.commandQueueSize())
                if (harvestInput("You have unexecuted commands.  Execute them? [Y]") == "Y") MasterHandler.CommandHandler.processCommands();
                else MasterHandler.CommandHandler.clearCommands();
            return;
        default:
            continue;
        }
    }
}
void CLI_Handler::beginCLILoop() {
    MasterHandler.CommandHandler.clearCommands();
    CLI_Loop();
}
#pragma endregion

#pragma region Command_Handler
void Command_Handler::processCommand(const Command& command) {
    switch (command.type) {
    case Command::TYPE::DRAW:
        MasterHandler.CanvasHandler.processDrawCommand(command);
        break;
    case Command::TYPE::MOVE:
        MasterHandler.CanvasHandler.processMoveCommand(command);
        break;
    case Command::TYPE::META:
        MasterHandler.CanvasHandler.processMetaCommand(command);
        break;
    case Command::TYPE::APP:
        MasterHandler.CLIHandler.beginCLILoop();
        break;
    }
}
void Command_Handler::processCommands() {
    for (const auto& command : commandQueue) processCommand(command);
    MasterHandler.CanvasHandler.refreshCursor();
    MasterHandler.SDLHandler.refreshPresent(MasterHandler.CanvasHandler.canvas, MasterHandler.CanvasHandler.width);
    clearCommands();
}
#pragma endregion

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