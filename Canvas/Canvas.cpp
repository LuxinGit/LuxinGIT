#include "Canvas.h"
#include "Application/Application.h"

luxel::luxel() { colour = DEFAULT_BACKGROUND_COLOUR; }
luxel::luxel(const std::array<uint8_t, 4>& c) : colour(c) {}

void luxel::resetLuxel() {
    colour = DEFAULT_BACKGROUND_COLOUR;
}

namespace Canvas {
    
    namespace {

        size_t indexFromCoord(const coordinate& c) {
            return size_t(c.y) * DEFAULT_CANVAS_WIDTH_MAX + size_t(c.x);
        }
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