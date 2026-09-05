#include "Draw.h"
#include "Application/Application.h"

#include <cassert>

namespace Draw {
    namespace {

        void drawCircle(
            Application_State& mh,
            const coordinate& c,
            const colour& col,
            int radius,
            const bool fill); // We forward declare this, as otherwise drawPoint cannot find drawCircle that it uses for drawing the pen :)

        colour getRandomColour() {
            return { static_cast<uint8_t>(rand() % 256),static_cast<uint8_t>(rand() % 256),static_cast<uint8_t>(rand() % 256), 255 };
        }

        void drawPoint (Application_State& mh, luxel& p, const colour& newColour) {
            if (p.colour == newColour) return;
            Action::markChangedPixel(mh.ActionState, &p, p.colour);
            p.colour = newColour;
            if (p.colour != mh.DrawState.backgroundColour) mh.DrawState.pixelsDrawn++;
        }
        void drawPoint (Application_State& mh, luxel* p, const colour& colour) {
            if (!p) return;
            drawPoint(mh, *p, colour);
        }
        void drawPoint (Application_State& mh, const coordinate& c, const colour& colour) {
            drawPoint(mh, Canvas::getLuxelFromCoord(mh.CanvasState, c, true), colour);
        }
        void drawPoint (Application_State& mh, const coordinate& c, const colour& colour, const bool useP) {
            if (useP) drawCircle(mh, c, colour, mh.DrawState.pen, true);
            else drawPoint(mh, c, colour);
            if (mh.DrawState.penMode == Draw_State::PEN_MODE::RAINBOW)
                if (mh.DrawState.pixelsDrawn > mh.DrawState.pixelsToRainbow) {
                    mh.DrawState.drawColour = getRandomColour();
                    mh.DrawState.pixelsDrawn = 0;
                }
        }
        void drawLine  (Application_State& mh, const coordinate& origin, const coordinate& destination, const colour& col, const bool useP) {

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

                drawPoint(mh, { x0, y0 }, col, useP);

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
        void drawCircle(
            Application_State& mh, 
            const coordinate& c, 
            const colour& col,
            int radius, 
            const bool fill) {

            int x = 0;
            int y = radius;
            int d = 1 - radius;

            while (x <= y) {

                if (fill) {
                    drawLine(mh, { c.x - x, c.y + y }, { c.x + x, c.y + y }, col, false);
                    drawLine(mh, { c.x - x, c.y - y }, { c.x + x, c.y - y }, col, false);
                                                                            
                    drawLine(mh, { c.x - y, c.y + x }, { c.x + y, c.y + x }, col, false);
                    drawLine(mh, { c.x - y, c.y - x }, { c.x + y, c.y - x }, col, false);
                }
                else {
                    drawPoint(mh, coordinate{ c.x + x, c.y + y }, col, true);
                    drawPoint(mh, coordinate{ c.x - x, c.y + y }, col, true);
                    drawPoint(mh, coordinate{ c.x + x, c.y - y }, col, true);
                    drawPoint(mh, coordinate{ c.x - x, c.y - y }, col, true);
                                                                  col,
                    drawPoint(mh, coordinate{ c.x + y, c.y + x }, col, true);
                    drawPoint(mh, coordinate{ c.x - y, c.y + x }, col, true);
                    drawPoint(mh, coordinate{ c.x + y, c.y - x }, col, true);
                    drawPoint(mh, coordinate{ c.x - y, c.y - x }, col, true);
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

        void fill(Application_State& mh, const coordinate& oc, const colour& nColour) {

            luxel* o = Canvas::getLuxelFromCoord(mh.CanvasState, oc, true);
            if (!o) return; // origin could not be on the display, and if so we don't want to fill around it.
            // this behaviour chosen instead of filling the pixel and then returning, fill should not be used as alias for drawPoint.
            colour oColour = o->colour;
            if (oColour == nColour) return;

            std::vector<coordinate> pixelStack = { oc };

            while (!pixelStack.empty()) {

                coordinate c = pixelStack.back();
                luxel* l = Canvas::getLuxelFromCoord(mh.CanvasState, c, true);

                if (!l or l->colour != oColour) {
                    pixelStack.pop_back();
                    continue;
                }

                drawPoint(mh, *l, nColour);

                pixelStack.pop_back();
                pixelStack.emplace_back(c.x + 1, c.y);
                pixelStack.emplace_back(c.x - 1, c.y);
                pixelStack.emplace_back(c.x, c.y + 1);
                pixelStack.emplace_back(c.x, c.y - 1);

            }

        }

    }

    void drawLineToNewCursor(Application_State& mh) {
        drawLine(mh, mh.CursorState.cursor, mh.CursorState.deltaCursor, *mh.DrawState.activeColour, true);
    }

    void processChangePenWidth(Application_State& mh, Command::Cmd& command) {

        Draw_State& s = mh.DrawState;

        int pen = s.pen;
        int payload = std::get<int>(command.args[0]);

        if (command.ID == COMMAND_ID::PEN_WIDTH_INCREASE)
            pen += payload;
        else if (command.ID == COMMAND_ID::PEN_WIDTH_DECREASE)
            pen -= payload;
        else if (command.ID == COMMAND_ID::PEN_SET)
            pen = payload;

        pen = std::clamp(
            pen,
            DEFAULT_PENWIDTH_MIN,
            DEFAULT_PENWIDTH_MAX
        );

        std::swap(s.pen, pen);

        command.ID = COMMAND_ID::PEN_SET;
        command.args = { pen };
    }
    void processCircle(Application_State& s, Command::Cmd& command) {
        //0:RADIUS 1:CENTRE_POINT 2:OUTLINE_COLOUR 3:FILL [INT]

        int radius =    10 * s.CursorState.drawStep;
        coordinate centre =    s.CursorState.cursor;
        colour c =        *s.DrawState.activeColour;
        bool f = false;

        if (int* rad = std::get_if<int>(&command.args[0])) 
            radius = *rad;
        if (coordinate* coor = std::get_if<coordinate>(&command.args[1]))
            centre = *coor;
        if (colour* outC = std::get_if<colour>(&command.args[2]))
            c = *outC;
        if (int* fill = std::get_if<int>(&command.args[3]))
            f = static_cast<bool>(*fill);

        drawCircle(s, centre, c, radius, f);

    }
    void processFill(Application_State& s, Command::Cmd& command) {
        
        colour c;
        coordinate coor = s.CursorState.cursor;
        
        switch (command.ID) {
        case(COMMAND_ID::DRAW_FILL_PAYLOAD):
            if (auto* arg = std::get_if<colour>(&command.args[0]))
                c = *arg;
            else c = *s.DrawState.activeColour;
            break;
        default:
            assert(false);
            return;
        }

        if (auto* arg = std::get_if<coordinate>(&command.args[1]))
            coor = *arg;

        fill(s, coor, c);

    }
    void processPenDown(Application_State& mh, Command::Cmd& command) {
        
        Draw_State& s = mh.DrawState;
        
        if (command.ID == COMMAND_ID::PEN_HELD_DOWN) {
            s.penDown = true;
            s.penContinuous = true;
        }
        else if (command.ID == COMMAND_ID::PEN_DOWN) 
            s.penDown = !s.penDown;

        if (s.penDown) drawPoint(mh, mh.CursorState.cursor, *s.activeColour, true);
    }
    void processChangePenMode(Application_State& mh, Command::Cmd& command) {
        Draw_State& s = mh.DrawState;

        switch (command.ID) {
        case(COMMAND_ID::PENMODE_DRAW):
            s.activeColour = &s.drawColour;
            s.penMode = Draw_State::PEN_MODE::DRAW;
            command.ID = COMMAND_ID::PENMODE_RUBBER;
            break;
        case(COMMAND_ID::PENMODE_RUBBER):
            s.activeColour = &s.backgroundColour;
            s.penMode = Draw_State::PEN_MODE::RUBBER;
            command.ID = COMMAND_ID::PENMODE_DRAW;
            break;
        case(COMMAND_ID::PENMODE_RAINBOW):
            // if payload has anything in it, check if thats different from what we have right now [for pixelsToRainbow].
            // and if so, set the pixels to rainbow == payload, and make sure we're in rainbow mode.
            // otherwise just make sure we flip between draw and rainbow.  all other penmodes disregarded.
            // this isn't a great implementation but when I come to revisit drawing modes we can figure it out then.
            s.activeColour = &s.drawColour;

            if (!command.args.empty()) {
                int payload = std::get<int>(command.args[0]);
                if (payload != s.pixelsToRainbow) 
                {
                    std::swap(payload, s.pixelsToRainbow);
                    s.penMode = Draw_State::PEN_MODE::RAINBOW;
                    return;
                }
            }
            if (s.penMode == Draw_State::PEN_MODE::RAINBOW)
                s.penMode = Draw_State::PEN_MODE::DRAW;
            else
                s.penMode = Draw_State::PEN_MODE::RAINBOW;

            return;

        default:
            assert(false);
        }
    }
    void processChangeColour(Application_State& mh, Command::Cmd& command) {
        Draw_State& s = mh.DrawState;
        colour c = {};
        colour* targetColour = s.activeColour;

        switch (command.ID) {
        case COMMAND_ID::COLOUR_SET_DEFAULT: 
            if (s.activeColour == &s.drawColour)  c = DEFAULT_DRAW_COLOUR;        
            else c = DEFAULT_BACKGROUND_COLOUR;  
            break;
        case COMMAND_ID::COLOUR_SET_DRAW:
            c = std::get<colour>(command.args[0]);
            targetColour = &s.drawColour;
            break;
        case COMMAND_ID::COLOUR_SET_BACKGROUND:
            c = std::get<colour>(command.args[0]);
            targetColour = &s.backgroundColour;
            break;
        case COMMAND_ID::COLOUR_SET_PICK:
            c = Canvas::getLuxelFromCoord(mh.CanvasState, mh.CursorState.cursor)->colour;
            break;
        case COMMAND_ID::COLOUR_SET_RANDOM:
            c = getRandomColour();
            break;
        default:
            assert(false); //stupidass
        }

        if (targetColour == &s.drawColour)
            command.ID = COMMAND_ID::COLOUR_SET_DRAW;
        else
            command.ID = COMMAND_ID::COLOUR_SET_BACKGROUND;

        std::swap(c, *targetColour);
        command.args = { c };

    }
    void processClearCanvas(Application_State& mh, Command::Cmd& command) {
        for (size_t y = 0; y < mh.CanvasState.height; ++y) {
            const size_t rowStart = y * DEFAULT_CANVAS_WIDTH_MAX;

            for (size_t i = rowStart; i < rowStart + mh.CanvasState.width; ++i) {
                drawPoint(mh, mh.CanvasState.canvas[i], mh.DrawState.backgroundColour);
            }
        }
    }
}