#pragma once

#include <SFML/System/Vector2.hpp>

struct Vec2i
{
    int x, y;

    inline Vec2i()
        : x(0)
        , y(0) {};
    inline Vec2i(const int& xy)
        : x(xy)
        , y(xy) {};
    inline Vec2i(const int& x, const int& y)
        : x(x)
        , y(y) {};
    Vec2i(const Vec2i& other)
        : x(other.x)
        , y(other.y) {}

    bool operator==(const Vec2i& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    Vec2i operator+(const Vec2i& rhs) const
    {
        return Vec2i(x + rhs.x, y + rhs.y);
    }

    Vec2i operator-(const Vec2i& rhs) const
    {
        return Vec2i(x - rhs.x, y - rhs.y);
    }

    Vec2i operator*(const int& rhs) const
    {
        return Vec2i(x * rhs, y * rhs);
    }

    Vec2i operator/(const int& rhs) const
    {
        return Vec2i(x / rhs, y / rhs);
    }

    Vec2i operator=(const Vec2i& rhs)
    {
        x = rhs.x;
        y = rhs.y;

        return *this;
    }

    bool within(Vec2i a, Vec2i size) const
    {
        return x > a.x && x < a.x+size.x && y > a.y && y < a.y+size.y;
    }

    inline sf::Vector2f to_sf_vec() const
    {
        return sf::Vector2f(x, y);
    }

    /*std::size_t operator()(const Vec2i& vec) const
    {
        std::size_t hash = 0x85ebca6b * x + 0xcc9e2d51 * y;
        return hash;
    }*/

    static const Vec2i Zero;
};

const Vec2i Vec2i::Zero(0, 0);
