#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>

const int milliDelta = 10;

enum Dir { UP, LEFT, DOWN, RIGHT };

struct Vec2i
{
    int x, y;

    Vec2i()
        : x(0)
        , y(0) {};
    Vec2i(const int& x, const int& y)
        : x(x)
        , y(y) {};
    Vec2i(const Vec2i& other)
        : x(other.x)
        , y(other.y) {}

    bool operator==(const Vec2i& vec) const
    {
        return x == vec.x && y == vec.y;
    }

    Vec2i operator+(const Vec2i& vec)
    {
        return Vec2i(this->x + vec.x, this->y + vec.y);
    }

    Vec2i operator-(const Vec2i& vec)
    {
        return Vec2i(this->x - vec.x, this->y - vec.y);
    }

    Vec2i operator*(const int& other)
    {
        return Vec2i(this->x * other, this->y * other);
    }

    Vec2i operator/(const int& other)
    {
        return Vec2i(this->x / other, this->y / other);
    }

    bool within(Vec2i a, Vec2i size)
    {
        return x > a.x && x < a.x+size.x && y > a.y && y < a.y+size.y;
    }

    std::size_t operator()(const Vec2i& vec) const
    {
        std::size_t hash = 0x85ebca6b * x + 0xcc9e2d51 * y;
        return hash;
    }
};

void update()
{
}

int main()
{
    int viewSize = 21;
    int milliUpdate = 100;
    bool running = false;
    sf::Clock clk;

    sf::RenderWindow win(sf::VideoMode(600, 600), "ant", sf::Style::Titlebar);
    sf::View view(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(viewSize, viewSize));

    win.setView(view);

    std::vector<Vec2i> tiles;
    Vec2i ant(0, 0);
    int direction = UP;

    while (win.isOpen())
    {
        sf::Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.delta < 0)
                    view.zoom(1.6f);
                else
                    view.zoom(0.625f);

                win.setView(view);
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                int code = event.key.code;

                if (code == sf::Keyboard::Up)
                    milliUpdate = std::max(milliUpdate - milliDelta, 0);
                if (code == sf::Keyboard::Down)
                    milliUpdate += milliDelta;
                if (code == sf::Keyboard::Space)
                    running = !running;
            }
            else if (event.type == sf::Event::Closed)
                win.close();
        }

        win.clear(sf::Color::White);

        if (running && clk.getElapsedTime().asMilliseconds() >= milliUpdate)
        {
            clk.restart();

            // ----- UPDATE GAME ----- //
            // black tile
            auto it_pos = std::find(tiles.begin(), tiles.end(), ant);
            if (it_pos != tiles.end())
            {
                direction = (direction + 1) % 4;
                tiles.erase(it_pos);
            }
            // white tile
            else
            {
                direction = (direction == 0) ? 3 : (direction - 1);
                tiles.push_back(ant);
            }

            if (direction == UP)
                ++ant.y;
            else if (direction == LEFT)
                --ant.x;
            else if (direction == DOWN)
                --ant.y;
            else if (direction == RIGHT)
                ++ant.x;
            // ----- ----------- ----- //
        }

        // ----- DRAW SQUARES ----- //
        sf::RectangleShape sq(sf::Vector2f(1, 1));
        sq.setFillColor(sf::Color(7, 40, 100));

        for (auto it = tiles.begin(); it != tiles.end(); ++it)
        {
            sq.setPosition(sf::Vector2f(it->x, it->y));
            win.draw(sq);
        }

        sq.setPosition(sf::Vector2f(ant.x+0.125, ant.y+0.125));
        sq.setFillColor(sf::Color(170, 0, 0));
        sq.setSize(sf::Vector2f(0.75, 0.75));

        win.draw(sq);
        // ----- ------------ ----- //

        win.display();
    }

    return 0;
}
