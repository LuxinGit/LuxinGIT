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

    void processChangePenWidth(Application_State& aS, Command::Cmd& command) {
        // 0:INT Setting {ADD,SET}, 1:INT Delta

        Draw_State& s = aS.DrawState;
        int& p = std::get<int>(command.args[1]);

        if (std::get<int>(command.args[0]) == 0) {
            p += s.pen;
            command.args[0] = 1;
        }

        p = std::clamp(p, DEFAULT_PENWIDTH_MIN, DEFAULT_PENWIDTH_MAX);
        
        std::swap(p, s.pen);

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
        
        colour c = *s.DrawState.activeColour;
        coordinate coor = s.CursorState.cursor;

        if (auto* arg = std::get_if<colour>(&command.args[0]))
            c = *arg;
        if (auto* arg = std::get_if<coordinate>(&command.args[1]))
            coor = *arg;

        fill(s, coor, c);

    }
    void processPenDown(Application_State& mh, Command::Cmd& command) {
        // 0 INT: Type {DISCRETE,CONTINUOUS}

        Draw_State& s = mh.DrawState;
        if (std::get<int>(command.args[0]))
            s.penContinuous = true;
        s.penDown = !s.penDown;

        if (s.penDown) drawPoint(mh, mh.CursorState.cursor, *s.activeColour, true);
    }
    void processChangePenMode(Application_State& mh, Command::Cmd& command) {
        //0 INT: Penmode to set to {DRAW,RUBBER,RAINBOW}.
        //1 INT: Additional optional parameter.

        Draw_State& s = mh.DrawState;
        using mode = Draw_State::PEN_MODE;
        mode p = static_cast<mode>(std::get<int>(command.args[0]));

        if (p == mode::RAINBOW) {
            if (int* i = std::get_if<int>(&command.args[1]);
                i && *i != s.pixelsToRainbow)
                    s.pixelsToRainbow = *i;
                //// This isn't perfect, as technically the command loses its invariance principle by dropping the previous state of pixelsToRainbow.
                //// The easiest solution is to separate the penmode commands but given that the primary state change is the same I don't love that.
                //// The best solution is one we have implemented commands with commands as arguments, construct two commands encoding the state change.
                        //// The reality is that the odds of needing good undo/redo functionality for this is low. 
            else p = (p == s.penMode) ? mode::DRAW : mode::RAINBOW;
        }

        std::swap(p, s.penMode);

    }

    namespace {
        colour* resolveActiveColourFromArg(Draw_State& s, int& target) 
        {
            switch (target)
            {
            case 0:
                if (s.activeColour == &s.drawColour) target = 1;
                else target = 2;
                return s.activeColour;
            case 1:
                return &s.drawColour;
            case 2:
                return &s.backgroundColour;
            }
        }
        colour resolveReplacementColourFromArg(Canvas_State& cS, Cursor_State& curS, int setting, int target, const Command::argument* ARG_2) 
        {
            switch (setting)
            {
            case 0:
            {            
                if (const colour* c = std::get_if<colour>(ARG_2))
                    return *c;
                else
                {
                    assert(false);
                    return { 255, 0, 0, 255 };
                }       
            }
            case 1:
                return getRandomColour();
            case 2:
                return Canvas::getLuxelFromCoord(cS, curS.cursor)->colour;
            default:
                return target == 1 ? DEFAULT_DRAW_COLOUR : DEFAULT_BACKGROUND_COLOUR;
            }
        }
    }
    void processChangeColour(Application_State& mh, Command::Cmd& command) {
        
        //0 INT: targetColour { activeColour, drawColour, backgroundColour }
        //1 INT: colourSource    { ARG_2, random, underCursor, default }
        //2 COLOUR: colour - only required if newColour == 0, as then implied that commmand carries replacement.

        Draw_State& s = mh.DrawState;

        
        
        int& target = std::get<int>(command.args[0]);
        colour* targetC = resolveActiveColourFromArg(s, target);
        colour replacementC = resolveReplacementColourFromArg
            (
                mh.CanvasState,
                mh.CursorState,
                std::get<int>(command.args[1]),
                target,
                &command.args[2]
            );

        std::swap(replacementC, *targetC);
        command.args = { target, 0, replacementC };

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