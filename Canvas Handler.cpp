#include "Canvas Handler.h"

luxel::luxel() { colour = DEFAULT_BACKGROUND_COLOUR; }
void luxel::resetLuxel() {
    colour = DEFAULT_BACKGROUND_COLOUR;
}


Canvas_Handler::Canvas_Handler(int& varwidth, int& varheight) :
    width(varwidth), height(varheight),
    CursorHandler(*this),
    DrawHandler(*this),
    canvas(std::vector<luxel>(width* height)) {
    // Only concern here is that if underlying height / width changes then this will need to be recalculated.
}
void Canvas_Handler::clearCanvas() {
    for (luxel& l : canvas) {
        l.resetLuxel();
    }
}

size_t Canvas_Handler::indexFromCoord(const std::pair<float, float>& c, const int& w) {
    // ASSUMES POSITIVE X/Y. INDEXING WITH THIS INDEX WITHOUT SIZE CHECKING MAY CAUSE OUT OF BOUNDARY MEMORY CRASH [IF COORD > LAST LUXEL INDEX].
    return size_t(c.second) * w + size_t(c.first);
}
size_t Canvas_Handler::indexFromCoord(const std::pair<float, float>& c)  {
    return indexFromCoord(c, width);
}

bool Canvas_Handler::coordCheck(const std::pair<float, float>& c) const {
    if (c.first < 0 or c.first > width) return false;
    if (c.second < 0 or c.second >= height) return false;
    return true;
} 
std::pair<float, float> Canvas_Handler::addCoords(const std::pair<float, float>& c1, const std::pair<float, float>& c2) {
    return { c1.first + c2.first, c1.second + c2.second };
}

luxel* Canvas_Handler::retrieveLuxelFromIndex(const size_t& index) { return &canvas[index]; }
luxel* Canvas_Handler::retrieveLuxelFromPoint(const std::pair<float, float>& c, bool coordCheck) { return &canvas[indexFromCoord(c)]; }
luxel* Canvas_Handler::retrieveLuxelFromPoint(const std::pair<float, float>& c) { return (coordCheck(c) ? retrieveLuxelFromPoint(c, true) : nullptr); }

void Canvas_Handler::processMetaCommand(const Command& command) {
    using Action = Command::META::ACTION;
    switch (static_cast<Action>(command.action)) {
        case Action::RESET:
            switch (static_cast<Command::META::RESET>(command.setting)) {
            case Command::META::RESET::RESET_CANVAS:
                clearCanvas();
                break;
            case Command::META::RESET::RESET_CURSOR:
                CursorHandler.cursor =      DEFAULT_CURSOR_POINT;
                CursorHandler.origin =      DEFAULT_CURSOR_POINT;
                CursorHandler.drawStep =    DEFAULT_DRAWSTEP;
                DrawHandler.pen =           DEFAULT_PENWIDTH;
                break;
            }
            break;
        case Action::CHANGE_COLOUR:
            DrawHandler.processChangeColourCommand(command);
            break;
        case Action::CHANGE_DRAWSTEP:
            CursorHandler.processChangeDrawstepCommand(command);
            break;
        case Action::ENABLE_RAINBOW:
            DrawHandler.processRainbowModeCommand(command);
            break;
        case Action::CHANGE_PEN_WIDTH:
            DrawHandler.processChangePenWidthCommand(command);
            break;
        case Action::PEN_DOWN:
            CursorHandler.penDown = !CursorHandler.penDown;
            break;
        case Action::SAVE_ORIGIN:
            CursorHandler.origin = CursorHandler.cursor;
            break;
        }
};
void Canvas_Handler::processMoveCommand(const Command& command) {
    CursorHandler.processMoveCommand(command);
}