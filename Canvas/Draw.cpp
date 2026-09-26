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

        void markStateChanges (Action_State& aS, Draw_State& dS, luxel& l) 
        {
            Action::markChangedPixel(aS, &l, l.colour);
            if (dS.penMode != Draw_State::PEN_MODE::RUBBER)
                dS.pixelsDrawn++;
        }

        void drawPoint (Application_State& mh, luxel& p, const colour& newColour) {
            if (p.colour == newColour) return;
            markStateChanges(mh.ActionState, mh.DrawState, p);
            p.colour = newColour;
        }
        void drawPoint (Application_State& mh, const coordinate& c, const colour& col) {
            if (auto* p = mh.ObjectState.activeObjectEdit) 
            {
                p->min.x = std::min(p->min.x, c.x);
                p->min.y = std::min(p->min.y, c.y);
                p->max.x = std::max(p->max.x, c.x);
                p->max.y = std::max(p->max.y, c.y);
            }
            luxel* l = Canvas::getDisplayedLuxelFromActiveCanvas(mh.CanvasState, c);
            if (!l) return;
            drawPoint(mh, *l, col);
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

            luxel* o = Canvas::getDisplayedLuxelFromActiveCanvas(mh.CanvasState, oc);
            if (!o) return; // origin could not be on the display, and if so we don't want to fill around it.
            // this behaviour chosen instead of filling the pixel and then returning, fill should not be used as alias for drawPoint.
            colour oColour = o->colour;
            if (oColour == nColour) return;

            std::vector<coordinate> pixelStack = { oc };

            while (!pixelStack.empty()) {

                coordinate c = pixelStack.back();
                luxel* l = Canvas::getDisplayedLuxelFromActiveCanvas(mh.CanvasState, c);

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

    void processChangePenWidth(Application_State& aS, Command::Cmmd& command) {
        // 0:INT Setting {ADD,SET}, 1:INT Delta

        Draw_State& s = aS.DrawState;
        int& p = std::get<int>(command.args[1]);

        if (std::get<int>(command.args[0]) == 0) {
            if (p)
                p = ++s.pen;
            else
                p = --s.pen;
            command.args[0] = 1;
        }

        p = std::clamp(p, DEFAULT_PENWIDTH_MIN, DEFAULT_PENWIDTH_MAX);
        
        std::swap(p, s.pen);

    }
    void processCircle(Application_State& s, Command::Cmmd& command) {
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
    void processFill(Application_State& s, Command::Cmmd& command) {
        
        colour c = *s.DrawState.activeColour;
        coordinate coor = s.CursorState.cursor;

        if (auto* arg = std::get_if<colour>(&command.args[0]))
            c = *arg;
        if (auto* arg = std::get_if<coordinate>(&command.args[1]))
            coor = *arg;

        fill(s, coor, c);

    }
    void processPenDown(Application_State& mh, Command::Cmmd& command) {
        // 0 INT: Type {DISCRETE,CONTINUOUS}

        Draw_State& s = mh.DrawState;
        if (std::get<int>(command.args[0]))
            s.penContinuous = true;
        s.penDown = !s.penDown;

        if (s.penDown) drawPoint(mh, mh.CursorState.cursor, *s.activeColour, true);
    }
    void processChangePenMode(Application_State& mh, Command::Cmmd& command) {
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
            default:
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
                return Canvas::getDisplayedLuxelFromActiveCanvas(cS, curS.cursor)->colour;
            default:
                return target == 1 ? DEFAULT_DRAW_COLOUR : DEFAULT_BACKGROUND_COLOUR;
            }
        }
    }
    void processChangeColour(Application_State& mh, Command::Cmmd& command) {
        
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

    void processClearCanvas(Application_State& mh, bool rActions) {
        
        Canvas_State& cS = mh.CanvasState;

        colour c = (cS.activeCanvas == &cS.displayCanvas) ? (mh.DrawState.backgroundColour) : colour{ 0, 0, 0, 0 };

        if (rActions) {
            *cS.activeCanvas = std::vector<luxel>(DEFAULT_CANVAS_SIZE_MAX, c);
            return;
        }

        for (size_t y = 0; y < cS.height; ++y) {
            const size_t rowStart = y * DEFAULT_CANVAS_WIDTH_MAX;

            for (size_t i = rowStart; i < rowStart + cS.width; ++i) {
                drawPoint(mh, (*cS.activeCanvas)[i], c);
            }
        }
    }
}

namespace Draw::Circle
{
    namespace Interpreter
    {
        static std::vector<Command::cmd> interpretCircle(const Application_State& s, const Command::cmd& c)
        {
            Command::cmd nc{ &DRAW_CIRCLE };

            const int* radius = std::get_if<int>(&c.args.at("Radius"));
            const coordinate* centre = std::get_if<coordinate>(&c.args.at("Centre"));
            const colour* col = std::get_if<colour>(&c.args.at("Colour"));
            const int* fill = std::get_if<int>(&c.args.at("Fill"));

            nc.setArg("Radius", radius ? *radius : 10 * s.CursorState.drawStep);
            nc.setArg("Centre", centre ? *centre : coordinate{ s.CursorState.cursor });
            nc.setArg("Colour", col ? *col : *s.DrawState.activeColour);
            nc.setArg("Fill", fill ? *fill : 0);

            return { std::move(nc) };
        }
    }

    namespace Processor
    {
        static void processCircle(Application_State& s, Command::cmd& c)
        {
            int radius = std::get<int>(c.args.at("Radius"));
            coordinate centre = std::get<coordinate>(c.args.at("Centre"));
            colour col = std::get<colour>(c.args.at("Colour"));
            bool fill = static_cast<bool>(std::get<int>(c.args.at("Fill")));

            drawCircle(s, centre, col, radius, fill);
        }
    }
    
    const Command::dfn DRAW_CIRCLE =
    {
        .name = "Circle",

        .argDefinitions =
        {
            {
                "Radius",
                Command::argmd
                {
                    .type = Command::Argument::ARGTYPE::INT,
                    .defaultValue = std::monostate(),
                    .validator = nullptr,
                    .required = false
                }
            },
            {
                "Centre",
                Command::argmd
                {
                    .type = Command::Argument::ARGTYPE::COORDINATE,
                    .defaultValue = std::monostate(),
                    .validator = nullptr,
                    .required = false
                }
            },
            {
                "Colour",
                Command::argmd
                {
                    .type = Command::Argument::ARGTYPE::COLOUR,
                    .defaultValue = std::monostate(),
                    .validator = nullptr,
                    .required = false
                }
            },
            {
                "Fill",
                Command::argmd
                {
                    .type = Command::Argument::ARGTYPE::INT,
                    .defaultValue = std::monostate(),
                    .validator = nullptr,
                    .required = false
                }
            }
        },

        .interp = &Interpreter::interpretCircle,
        .prcssr = &Processor::processCircle
    };
    
}

namespace Draw::Fill
{
    namespace Interpreter
    {
        static std::vector<Command::cmd> interpretFill(const Application_State& s, const Command::cmd& c)
        {
            Command::cmd nc = { &DRAW_FILL };
            nc.args["Fill Colour"] = c.args.contains("Fill Colour")
                ? c.args.at("Fill Colour")
                : *s.DrawState.activeColour;

            nc.args["Coordinate"] = c.args.contains("Coordinate")
                ? c.args.at("Coordinate")
                : s.CursorState.cursor;

            return { nc };
        }
    }

    namespace Processor
    {
        static void processFill(Application_State& s, Command::cmd& c)
        {
            Draw::fill(
                s, 
                std::get<coordinate>(c.args.at("Coordinate")), 
                std::get<colour>(c.args.at("Fill Colour")));
        }
    }

    const Command::dfn DRAW_FILL =
    {
        .name = "Fill",

        .argDefinitions =
        {
            {
                "Coordinate",
                Command::argmd
                {
                    .type = Command::Argument::ARGTYPE::COORDINATE,
                    .defaultValue = std::monostate()
                }
            },
            {
                "Fill Colour",
                Command::argmd
                {
                    .type = Command::Argument::ARGTYPE::COLOUR,
                    .defaultValue = std::monostate()
                }
            }
        },

        .interp = &Interpreter::interpretFill,
        .prcssr = &Processor::processFill
    };

}

namespace Draw::Pen::Colour::Constants
{
    static const std::array<Command::argument, 2> validSources =
    {
        std::string{"Random"},
        std::string{"Cursor"}
    };

    static const std::array<Command::argument, 3> validTargets =
    {
        std::string{"Active"},
        std::string{"Draw"},
        std::string{"Background"}
    };

}

namespace Draw::Pen::Colour::SetSource
{
    namespace Interpreter
    {       
       

        static const colour retrieveColourFromSource(const Application_State& s, const std::string& src)
        {
            if (src == "Random")            
                return Draw::getRandomColour();

            assert(src == "Cursor");
                return Cursor::retrieveColourUnderCursor(s);
            
        }
       
        static std::vector<Command::cmd> interpretColourChangeInferred(const Application_State& s, const Command::cmd& c)
        {
            const std::string& src = std::get<std::string>(c.args.at("Source"));
            const colour& col = retrieveColourFromSource(s, src);

            Command::cmd nC{ &SetColour::DRAW_COLOUR_SETCOLOUR };
            
            nC.setArg("Colour", col);
            nC.setArg("Target", c.args.at("Target"));

            return { nC };
        }
    }

    const Command::dfn DRAW_COLOUR_SETSOURCE =
    {
        .name = "Change pen colour",

        .argDefinitions =
        {
            {
                "Source",
                Command::argmd
                {
                    .type = Command::Argument::ARGTYPE::STRING,
                    .defaultValue = std::monostate(),
                    .validator = &Command::Validator::genericValidator,
                    .required = true,
                    .constraints = Constants::validSources
                }
            },
            {
                "Target",
                Command::argmd
                {
                    .type = Command::Argument::ARGTYPE::STRING,
                    .defaultValue = std::monostate(),
                    .validator = &Command::Validator::genericValidator,
                    .required = true,
                    .constraints = Constants::validTargets
                }
            }
            
        },

        .interp = &Interpreter::interpretColourChangeInferred

    };
}

namespace Draw::Pen::Colour::SetColour
{

    namespace Processor
    {
        static colour* retrieveColourFromTarget(Application_State& s, const std::string& tar)
        {
            if (tar == "Active")            
                return s.DrawState.activeColour;            
            if (tar == "Draw")            
                return &s.DrawState.drawColour;            
            assert(tar == "Background");
                return &s.DrawState.backgroundColour;
        }

        static void processColourChange(Application_State& s, Command::cmd& c)
        {
            colour* col = retrieveColourFromTarget(s, std::get<std::string>(c.args.at("Target")));
            std::swap(*col, std::get<colour>(c.args.at("Colour")));
        }
    }

    const Command::dfn DRAW_COLOUR_SETCOLOUR =
    {
        .name = "Change Drawing Colour",

        .argDefinitions =
        {
            {
                "Colour",
                Command::argmd
                {
                    .type = Command::Argument::ARGTYPE::COLOUR,
                    .defaultValue = std::monostate()
                }
            },
            {
                "Target",
                Command::argmd
                {
                    .type = Command::Argument::ARGTYPE::STRING,
                    .defaultValue = std::monostate(),
                    .validator = &Command::Validator::genericValidator,
                    .required = true,
                    .constraints = Constants::validTargets
                }
            }
        },

        .prcssr = &Processor::processColourChange

    };
}