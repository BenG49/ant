#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <iostream>
#include <math.h>

using namespace sf;

const int milliDelta = 10;

RenderWindow win(VideoMode(600, 600), "ant", Style::Titlebar);
Clock clk;
int milliUpdate = 100;
bool running = false;

int viewSize = 21;

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

    bool operator == (const vec2i& vec) const
    {
        return x == vec.x && y == vec.y;
    }

    vec2i operator + (const vec2i& vec)
    {
        return vec2i(this->x + vec.x, this->y + vec.y);
    }

    vec2i operator - (const vec2i& vec)
    {
        return vec2i(this->x - vec.x, this->y - vec.y);
    }

    vec2i operator * (const int& other)
    {
        return vec2i(this->x * other, this->y * other);
    }

    vec2i operator / (const int& other)
    {
        return vec2i(this->x / other, this->y / other);
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

std::unordered_set<vec2i, vec2i> tiles({});
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

void draw()
{
    RectangleShape sq(Vector2f(1, 1));
    sq.setFillColor(Color::Black);
    sq.setOutlineThickness(0);

    for (auto it = tiles.begin(); it != tiles.end(); ++it)
    {
        sq.setPosition(Vector2f(it->x, it->y));
        win.draw(sq);
    }

    sq.setPosition(Vector2f(ant.x+0.125, ant.y+0.125));
    sq.setFillColor(Color::Red);
    sq.setSize(Vector2f(0.75, 0.75));

    win.draw(sq);
}

int main()
{
    Event pollEvent;
    View view(Vector2f(0, 0), Vector2f(viewSize, viewSize));

    win.setView(view);

    while (win.isOpen())
    {
        while (win.pollEvent(pollEvent))
        {
            if (pollEvent.type == Event::MouseWheelScrolled)
            {
                viewSize = std::max(viewSize - pollEvent.mouseWheelScroll.delta, 11.0f);
                if (viewSize != view.getSize().x)
                {
                    view.setSize(Vector2f(viewSize, viewSize));
                    win.setView(view);
                }
            }
            else if (pollEvent.type == Event::KeyPressed)
            {
                int code = pollEvent.key.code;

                if (code == Keyboard::Up)
                    milliUpdate = std::max(milliUpdate - milliDelta, 0);
                if (code == Keyboard::Down)
                    milliUpdate += milliDelta;
                if (code == Keyboard::Space)
                    running = !running;
            }
            else if (pollEvent.type == Event::Closed)
                win.close();
        }

        win.clear(Color::White);
        if (running && clk.getElapsedTime().asMilliseconds() >= milliUpdate)
        {
            update();
            clk.restart();
        }

        draw();
        win.display();
    }

    return 0;
}