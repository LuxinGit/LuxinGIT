#include "Canvas.h"
#include "Application/Application.h"

luxel::luxel() { colour = DEFAULT_BACKGROUND_COLOUR; }
luxel::luxel(const std::array<uint8_t, 4>& c) : colour(c) {}

void luxel::resetLuxel() {
    colour = DEFAULT_BACKGROUND_COLOUR;
}

namespace Canvas {
    
    namespace {

        size_t newIndexFromCoord(const coordinate& c, int width) {
            return size_t(c.y) * width + size_t(c.x);
        }
        void updateCanvasSize(Canvas_State& cS, SDL_State& sS, coordinate& c)
        {
            if (!newCoordCheck(c)) return;
            if (c.x) std::swap(cS.width, c.x);
            if (c.y) std::swap(cS.height, c.y);
            SDL_SetWindowSize(sS.Window, cS.width, cS.height);
        }

        

    }
    
    colour getColourFromCoordinate(const coordinate& c, const std::vector<luxel>& canvas, const int width)
    {
        // very hacky implementation, but basically wrapper for retrieving colour from given coordinate when caller does not own non-const canvas.
        // previous implementation for this method also hacky but in a different way...
        return canvas[newIndexFromCoord(c, width)].colour;
    }

    bool newCoordCheck(const coordinate& c, const coordinate& max)
    {
        return c >= coordinate{ 0, 0 } and c < max;
    }
    luxel* newGetLuxelFromCoord(std::vector<luxel>& v, const coordinate& c, const coordinate& dim)
    {
        // dim == dimensions of vector.
        if (!newCoordCheck(c, dim)) return nullptr;
        return &v[newIndexFromCoord(c, dim.x)];
    }
    luxel* getDisplayedLuxelFromActiveCanvas(Canvas_State& cS, const coordinate& c)
    {
        if (c.x >= cS.width or c.y >= cS.height) return nullptr;
        return Canvas::newGetLuxelFromCoord(
            *cS.activeCanvas,
            c,
            coordinate{ DEFAULT_CANVAS_WIDTH_MAX, DEFAULT_CANVAS_HEIGHT_MAX });
    }

    void swapActiveCanvas(Canvas_State& s)
    {
        s.activeCanvas = (s.activeCanvas == &s.displayCanvas) ? (&s.bufferCanvas) : (&s.displayCanvas);
    }
    
    void processCanvasSize(Application_State& mh, Command::Cmmd& command) {

        coordinate c{
            mh.CanvasState.width,
            mh.CanvasState.height
        };

        if (int* width = std::get_if<int>(&command.args[0]))
            c.x = *width;

        if (int* height = std::get_if<int>(&command.args[1]))
            c.y = *height;

        updateCanvasSize(
            mh.CanvasState,
            mh.SDLState,
            c
        );

        command.args = { c.x, c.y };
    }

}

namespace Canvas::Resize::Coordinate
{


    namespace Validator
    {
        static bool checkResizeWithinAllowedDimensions(const Command::argument& a, const Command::argmd&)
        {
            const coordinate& c = std::get<coordinate>(a);
            return newCoordCheck(c);
        }
    }


    namespace Interpreter
    {
        static std::vector<Command::cmd> interpretResize(const Application_State& s, const Command::cmd& c)
        {

            const coordinate coor = std::get<coordinate>(c.args.at("Size Coordinate"));

            Command::cmd setW{ &Width::CANVAS_RESIZE_WIDTH };
            setW.setArg("Width", coor.x);
            Command::cmd setH{ &Height::CANVAS_RESIZE_HEIGHT };
            setH.setArg("Height", coor.y);

            return { setW, setH };

        }
    }


    const Command::dfn CANVAS_RESIZE_COORDINATE =
    {
        .argDefinitions =
        {
            {
                "Size Coordinate",
                Command::argmd
                {
                    .type = Command::Argument::ARGTYPE::COORDINATE,
                    .defaultValue = std::monostate(),
                    .validator = &Validator::checkResizeWithinAllowedDimensions,
                }
            }
        },
        .interp = &Interpreter::interpretResize
    };

}

namespace Canvas::Resize::Width
{
    namespace Validator
    {
        static bool checkResizeWithinAllowedDimensions(const Command::argument& a, const Command::argmd&)
        {
            const int& i = std::get<int>(a);
            return i >= DEFAULT_CANVAS_WIDTH_MIN and i < DEFAULT_CANVAS_WIDTH_MAX;                
        }
    }


    namespace Processor
    {
        static void processResize(Application_State& s, Command::cmd& c)
        {
            std::swap(std::get<int>(c.args.at("Width")), s.CanvasState.width);
            SDL_SetWindowSize(s.SDLState.Window, s.CanvasState.width, s.CanvasState.height);
        }
    }

    const Command::dfn CANVAS_RESIZE_WIDTH =
    {
        .argDefinitions =
        {
            {
                "Width",
                Command::argmd
                {
                    .type = Command::Argument::ARGTYPE::INT,
                    .defaultValue = std::monostate(),
                    .validator = &Validator::checkResizeWithinAllowedDimensions,
                }
            }
        },
        .prcssr = &Processor::processResize
    };
}
namespace Canvas::Resize::Height
{
    namespace Validator
    {
        static bool checkResizeWithinAllowedDimensions(const Command::argument& a, const Command::argmd&)
        {
            const int& i = std::get<int>(a);
            return i >= DEFAULT_CANVAS_HEIGHT_MIN and i < DEFAULT_CANVAS_HEIGHT_MAX;
        }
    }


    namespace Processor
    {
        static void processResize(Application_State& s, Command::cmd& c)
        {
            std::swap(std::get<int>(c.args.at("Height")), s.CanvasState.height);
            SDL_SetWindowSize(s.SDLState.Window, s.CanvasState.width, s.CanvasState.height);
        }
    }

    const Command::dfn CANVAS_RESIZE_HEIGHT =
    {
        .argDefinitions =
        {
            {
                "Height",
                Command::argmd
                {
                    .type = Command::Argument::ARGTYPE::INT,
                    .defaultValue = std::monostate(),
                    .validator = &Validator::checkResizeWithinAllowedDimensions,
                }
            }
        },
        .prcssr = &Processor::processResize
    };
}
