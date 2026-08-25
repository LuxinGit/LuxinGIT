#include "Draw Handler.h"
#include "Master Handler.h"

void Draw_Handler::checkDrawData() {
    if (rainbowMode) if (CanvasHandler.CursorHandler.pixelsDrawn > pixelsToRainbow and penMode == PEN_MODE::DRAW) {
        drawColour = getRandomColour();
        CanvasHandler.CursorHandler.pixelsDrawn = 0;
    }
}

void Draw_Handler::drawPoint(const luxel* p) {
    if (!p) return;
}
void Draw_Handler::drawPoint(const coordinate& c) {
    luxel* ptr = CanvasHandler.getLuxelFromCoord(c);
    std::array<uint8_t, 4>& pixelColour = ptr->colour;
    CanvasHandler.MasterHandler.ActionHandler.pixelChange(c, pixelColour);
    pixelColour = *activeColour;
    CanvasHandler.CursorHandler.pixelsDrawn += 1;
}
void Draw_Handler::drawPoint(const coordinate& c, const bool useP) {
    if (useP) drawCircle(c, pen, true);
    else drawPoint(c);
    checkDrawData();
}


void Draw_Handler::drawLine(const coordinate& origin, const coordinate& destination, const bool useP) {

    int x0 = origin.x;
    int y0 = origin.y;
    int x1 = destination.x;
    int y1 = destination.y;

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
void Draw_Handler::drawCircle(const coordinate& c, int radius, const bool fill, const bool useP) {
    int x = 0;
    int y = radius;
    int d = 1 - radius;

    while (x <= y) {

        if (fill) {
            drawLine({ c.x - x, c.y + y }, { c.x + x, c.y + y }, false);
            drawLine({ c.x - x, c.y - y }, { c.x + x, c.y - y }, false);

            drawLine({ c.x - y, c.y + x }, { c.x + y, c.y + x }, false);
            drawLine({ c.x - y, c.y - x }, { c.x + y, c.y - x }, false);
        }
        else {
            drawPoint({ c.x + x, c.y + y }, useP);
            drawPoint({ c.x - x, c.y + y }, useP);
            drawPoint({ c.x + x, c.y - y }, useP);
            drawPoint({ c.x - x, c.y - y }, useP);

            drawPoint({ c.x + y, c.y + x }, useP);
            drawPoint({ c.x - y, c.y + x }, useP);
            drawPoint({ c.x + y, c.y - x }, useP);
            drawPoint({ c.x - y, c.y - x }, useP);
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

void Draw_Handler::fill(const coordinate& oc, const std::array<uint8_t, 4>& nColour) {
    
    const std::array<uint8_t, 4> oColour = CanvasHandler.getLuxelFromCoord(oc)->colour;

    if (oColour == nColour) return;

    std::vector<coordinate> pixelStack = { oc };

    while (!pixelStack.empty()) {

        coordinate c = pixelStack.back();
        luxel* l = CanvasHandler.getLuxelFromCoord(c);

        if (!l or l->colour != oColour) {
            pixelStack.pop_back(); 
            continue;
        }

        l->colour = nColour;
        pixelStack.pop_back();
        pixelStack.emplace_back(c.x + 1, c.y);
        pixelStack.emplace_back(c.x - 1, c.y);
        pixelStack.emplace_back(c.x, c.y + 1);
        pixelStack.emplace_back(c.x, c.y - 1);

    }

}

std::array<uint8_t, 4> Draw_Handler::getRandomColour() {
    return { static_cast<uint8_t>(rand() % 256),static_cast<uint8_t>(rand() % 256),static_cast<uint8_t>(rand() % 256), 255 };
}


void Draw_Handler::processCommand(const Command& command) {
    switch (command.type) {
    case Command::TYPE::META:
        processMetaCommand(command);
        break;
    case Command::TYPE::DRAW:
        processDrawCommand(command);
        break;
    }
}

    void Draw_Handler::processMetaCommand(const Command& command) {
        using Action = Command::META::ACTION;
        switch (static_cast<Action>(command.action)) {
        case Action::CHANGE_COLOUR:
            processChangeColourCommand(command);
            break;
        case Action::CHANGE_PENMODE:
            processChangePenModeCommand(command);
            break;
        case Action::CHANGE_PEN_WIDTH:
            processChangePenWidthCommand(command);
            break;
        }
    };
        void Draw_Handler::processChangeColourCommand(const Command& command) {
            switch (static_cast<Command::META::CHANGE_COLOUR>(command.setting)) {
            case Command::META::CHANGE_COLOUR::DEFAULT:
                drawColour = DEFAULT_DRAW_COLOUR;
                break;
            case Command::META::CHANGE_COLOUR::USE_PAYLOAD:
                drawColour = std::get<std::array<uint8_t, 4>>(command.payload);
                break;
            case Command::META::CHANGE_COLOUR::RANDOM:
                drawColour = getRandomColour();
                break;
            }
        }
        void Draw_Handler::processChangePenModeCommand(const Command& command) {
            using S = Command::META::CHANGE_PENMODE;

                switch (static_cast<S>(command.setting)) {
                case (S::DRAW):
                    activeColour = &drawColour;
                    break;
                case (S::RUBBER):
                    activeColour = &backgroundColour;
                    break;
                case (S::PEN_DOWN):
                    processPenDownCommand(command);
                    break;
                case (S::RAINBOW):
                    processRainbowModeCommand(command);
                    break;
                }
        }
            void Draw_Handler::processRainbowModeCommand(const Command& command) {
                if (auto p = std::get_if<int>(&command.payload)) {
                    int candidate = *p;
                    if (candidate != pixelsToRainbow) pixelsToRainbow = candidate;
                    else rainbowMode = !rainbowMode;
                }
                else rainbowMode = !rainbowMode;
            }
            void Draw_Handler::processPenDownCommand(const Command& command) {
                using Setting = Command::META::PENMODE_PDOWN_PINTERP;
                switch (static_cast<Setting>(std::get<int>(command.payload))) {
                    case (Setting::DISCRETE):
                        CanvasHandler.CursorHandler.penDown = !CanvasHandler.CursorHandler.penDown;
                        if (CanvasHandler.CursorHandler.penDown) drawPoint(CanvasHandler.CursorHandler.cursor, true);
                        break;
                    case (Setting::CONTINUOUS):
                        CanvasHandler.CursorHandler.penContinuous = true;
                        CanvasHandler.CursorHandler.penDown = true;
                        drawPoint(CanvasHandler.CursorHandler.cursor, true);
                        break;
                }
            }
        void Draw_Handler::processChangePenWidthCommand(const Command& command) {

            using Setting = Command::META::CHANGE_PEN_WIDTH;

            switch (static_cast<Setting>(command.setting)) {
            case (Setting::ADD_PAYLOAD):
                pen = std::clamp(pen + std::get<int>(command.payload), 1, 50);
                break;
            case (Setting::SET_TO_PAYLOAD):
                pen = std::clamp(std::get<int>(command.payload), 1, 50);
                break;
            }

        }

    void Draw_Handler::processDrawCommand(const Command& command) {
    using Action = Command::DRAW::ACTION;
    switch (static_cast<Action>(command.action)) {
    case Action::LINE:
        processDrawLineCommand(static_cast<Command::DRAW::LINE>(command.setting));
        break;
    case Action::CIRCLE:
        processDrawCircleCommand(command);
        break;
    case Action::POINT:
        processDrawPointCommand(command);
        break;
    case Action::FILL:
        processFillCommand(command);
        break;
    }

};
        void Draw_Handler::processDrawPointCommand(const Command& command) {
            using Setting = Command::DRAW::POINT;
            switch (static_cast<Setting>(command.setting)) {
            case Setting::USE_CURSOR:
                drawPoint(CanvasHandler.CursorHandler.cursor, true);
                break;
            case Setting::USE_PAYLOAD:
                drawPoint(std::get<std::pair<float, float>>(command.payload), true);
                break;
            }
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
                    drawColour = getRandomColour();
                    drawCircle(CanvasHandler.CursorHandler.cursor, i, false, true);
                }
                drawColour = { 200, 200, 200, 255 };
                return;
            }
            else drawCircle(CanvasHandler.CursorHandler.cursor, radius, false, true);
        }
        void Draw_Handler::processFillCommand(const Command& command) {
            using Setting = Command::DRAW::FILL;
            switch (static_cast<Setting>(command.setting)) {
            case Setting::USE_PAYLOAD:
                fill(CanvasHandler.CursorHandler.cursor, std::get<std::array<uint8_t, 4>>(command.payload));
                break;
            case Setting::USE_DRAW_COLOUR:
                fill(CanvasHandler.CursorHandler.cursor, *activeColour);
                break;
            }
        }



Draw_Handler::Draw_Handler(Canvas_Handler& CanvH) : CanvasHandler(CanvH) {}