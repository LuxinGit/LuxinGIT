#include "Draw Handler.h"
#include "Canvas Handler.h"

void Draw_Handler::checkDrawData() {
    if (rainbowMode) if (CanvasHandler.CursorHandler.pixelsDrawn > pixelsToRainbow) {
        colour = getRandomColour();
        CanvasHandler.CursorHandler.pixelsDrawn = 0;
    }
}

void Draw_Handler::drawPoint(const std::pair<float, float>& c) {
    luxel* ptr = CanvasHandler.retrieveLuxelFromPoint(c);
    if (!ptr) return;
    ptr->colour = colour;
    CanvasHandler.CursorHandler.pixelsDrawn += 1;
}

void Draw_Handler::drawLine(std::pair<int, int> origin, std::pair<int, int> destination, const bool useP) {

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

void Draw_Handler::drawCircle(const std::pair<float, float>& c, int radius, const bool fill) {
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

void Draw_Handler::usePen(const std::pair<float, float>& c) {
    drawCircle(c, pen, true);
    checkDrawData();
}
void Draw_Handler::drawPoint(const std::pair<float, float>& c, const bool useP) {
    if (useP) usePen(c);
    else drawPoint(c);
}

std::array<uint8_t, 4> Draw_Handler::getRandomColour() {
    return { static_cast<uint8_t>(rand() % 256),static_cast<uint8_t>(rand() % 256),static_cast<uint8_t>(rand() % 256), 255 };
}

void Draw_Handler::processDrawLineCommand(const Command::DRAW::LINE& setting) {
    // fizzbuzz
}
void Draw_Handler::processDrawCircleCommand(const Command& command) {
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
void Draw_Handler::processChangeColourCommand(const Command& command) {
    switch (static_cast<Command::META::CHANGE_COLOUR>(command.setting)) {
    case Command::META::CHANGE_COLOUR::DEFAULT:
        colour = { 200, 200, 200, 255 };
        break;
    case Command::META::CHANGE_COLOUR::USE_PAYLOAD:
        colour = std::get<std::array<uint8_t, 4>>(command.payload);
        break;
    case Command::META::CHANGE_COLOUR::RANDOM:
        colour = getRandomColour();
        break;
    }
}
void Draw_Handler::processRainbowModeCommand(const Command& command) {
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
void Draw_Handler::processChangePenWidthCommand(const Command& command) {
    pen = std::clamp(pen + std::get<int>(command.payload), 1, 50);
}

Draw_Handler::Draw_Handler(Canvas_Handler& CanvH) : CanvasHandler(CanvH) {}