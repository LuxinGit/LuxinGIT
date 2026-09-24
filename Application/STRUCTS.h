#pragma once

#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include "CONSTANTS.h"
#include <optional>

using colour = std::array<uint8_t, 4>;

struct luxel {

    luxel();
    luxel(const std::array<uint8_t,4>&);
    void resetLuxel();

    std::array<uint8_t, 4> colour;

private:
    static uint8_t floatToColour(float f) {
        return static_cast<uint8_t>(f * 255.0f + 0.5f);
    }
    static float colourToFloat(const uint8_t& c) {
        return static_cast<float>(c) / 255;
    }
public:
    static std::array<uint8_t, 4> floatsToColour(const std::array<float, 4>& floats) {
        std::array<uint8_t, 4> ret = {};
        for (int i = 0; i < 4; i++) {
            ret[i] = floatToColour(floats[i]);
        }
        return ret;
    }
    static std::array<float, 4> coloursToFloat(const std::array<uint8_t, 4>& colours) {
        std::array<float, 4> ret = {};
        for (int i = 0; i < 4; i++) {
            ret[i] = colourToFloat(colours[i]);
        }
        return ret;
    }
};


struct coordinate {
    int x = 0;
    int y = 0;

    bool operator==(const coordinate& other) const {
        return x == other.x and y == other.y;
    }

    bool operator<(const coordinate& other) const {
        return (x < other.x or y < other.y);
    }
    bool operator>(const coordinate& other) const {
        return (x > other.x or y > other.y); 
    }
    bool operator>=(const coordinate& other) const {
        return x >= other.x && y >= other.y;
    }
    bool operator<=(const coordinate& other) const {
        return x <= other.x && y <= other.y;
    }

    coordinate operator+(const coordinate& other) const {
        return { x + other.x, y + other.y };
    }
    coordinate operator-(const coordinate& other) const {
        return { x - other.x, y - other.y };
    }
    coordinate operator*(int i) const 
    {
        return { x * i, y * i };
    }

    operator std::pair<float, float>() const {
        return {
            static_cast<float>(x),
            static_cast<float>(y)
        };
    }

    operator std::string() const{
        return ("{" + std::to_string(x) + "," + std::to_string(y) + "}");
    }

    coordinate(const int& varx, const int& vary) : x(varx), y(vary) {}
    coordinate(const std::pair<int, int>& varC) : x(varC.first), y(varC.second) {}
    coordinate(const float& varfx, const float& varfy) : x(static_cast<int>(varfx)), y(static_cast<int>(varfy)) {}
    coordinate(const std::pair<float, float>& varfC) : coordinate(varfC.first, varfC.second) {}

};

struct coordinate_hash {
    std::size_t operator()(const coordinate& c) const noexcept {
        std::size_t h1 = std::hash<int>{}(c.x);
        std::size_t h2 = std::hash<int>{}(c.y);

        return h1 ^ (h2 << 1);
    }
};

namespace Direction
{
    inline const std::unordered_map<std::string, coordinate> dirmap =
    {
        { "up",    {  0, -1 } },
        { "down",  {  0,  1 } },
        { "left",  { -1,  0 } },
        { "right", {  1,  0 } }
    };

    inline coordinate get(const std::string& direction)
    {
        return dirmap.at(direction);
    }
}
