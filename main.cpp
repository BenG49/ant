#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <iostream>
#include <math.h>

using namespace sf;

RenderWindow win(VideoMode(600, 600), ".", Style::Titlebar);

enum dir { UP, LEFT, DOWN, RIGHT };

struct vec2i
{
    int x, y;

    vec2i() : x(0), y(0) {};
    vec2i(const int& x, const int& y) : x(x), y(y) {};
    vec2i(const vec2i& other)
    {
        x = other.x;
        y = other.y;
    }

    vec2i& operator = (const vec2i& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    bool operator == (const vec2i& vec) const
    {
        return x == vec.x && y == vec.y;
    }

    bool within(vec2i a, vec2i size)
    {
        return x > a.x && x < a.x+size.x && y > a.y && y < a.y+size.y;
    }

    std::size_t operator()(const vec2i& vec) const
    {
        std::size_t hash = 0x85ebca6b * x + 0xcc9e2d51 * y;
        return hash;
    }
};

std::unordered_set<vec2i, vec2i> tiles({ vec2i(5, 5) });
vec2i ant(0, 0);
int direction = UP;

void update()
{
    // black tile
    if (tiles.find(ant) != tiles.end())
    {
        direction = (direction + 1) % 4;
        tiles.erase(ant);
    }
    // white tile
    else
    {
        direction = (direction == 0) ? 3 : (direction - 1);
        tiles.insert(ant);
    }

    if (direction == UP)
        ++ant.y;
    else if (direction == LEFT)
        --ant.x;
    else if (direction == DOWN)
        --ant.y;
    else if (direction == RIGHT)
        ++ant.x;
}

/*void draw(vec2i center, int squareCountY)
{
    int squareSize = floor(480.0f/squareCountY);

    vec2i size = {
        floor(640.0f/squareSize),
        squareCountY
    };

    vec2i topLeft = {
        center.x - floor(size.x / 2),
        center.y - floor(size.y / 2)
    };

    for (auto it = tiles.begin(); it != tiles.end(); ++it)
    {
        vec2i temp = { it->x, it->y };
        // draw
        if (temp.within(topLeft, size))
        {
            
        }
    }
}*/

void d()
{
    vec2i topleft = { -10, -10 };
    vec2i size = { 20, 20 };
    // 30
    RectangleShape sq(Vector2f(30, 30));
    sq.setFillColor(Color::Black);
    sq.setOutlineThickness(0);

    for (auto it = tiles.begin(); it != tiles.end(); ++it)
    {
        vec2i temp = { it->x, it->y };
        // draw
        if (temp.within(topleft, size))
        {
            sq.setPosition(Vector2f(topleft.x + temp.x * 30, topleft.y + temp.y * 30));
            win.draw(sq);
        }
    }
}

int main()
{
    Event pollEvent;

    while (win.isOpen())
    {
        while (win.pollEvent(pollEvent))
            if (pollEvent.type == Event::Closed)
                win.close();

        win.clear(Color::White);
        d();
        win.display();
    }

    return 0;
}