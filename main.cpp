#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <iostream>
#include <math.h>

using namespace sf;

RenderWindow win(VideoMode(640, 480), ".", Style::Titlebar);

enum dir { UP, LEFT, DOWN, RIGHT };

struct vec2i
{
    int x, y;

    bool within(vec2i a, vec2i size)
    {
        return x > a.x && x < a.x+size.x && y > a.y && y < a.y+size.y;
    }
};

std::unordered_set<vec2i> tiles;
vec2i ant = { 0, 0 };
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

void draw(vec2i center, int squareCountY)
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
        if (temp.within(topLeft, size))
        {
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
        win.display();
    }

    return 0;
}