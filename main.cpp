/**
 * snek
 */

#include <iostream>
#include <queue>
#include <random>
#include <cmath>
#include <list>

#include <SFML/Graphics.hpp>

#include "vec2i.hpp"

class Game
{
    typedef enum {
        INVALID, UP, LEFT, DOWN, RIGHT
    } dir;

    typedef enum {
        START, PLAYING, LOST
    } state;

    static const Vec2i vec_dirs[5];

    static const Vec2i START_POS;
    static const int MAX_KEYPRESSES = 4;
    static constexpr int BOARD_SIZE = 17;

    int milli_update;
    state game_state;

    sf::RenderWindow win;
    sf::View view;
    sf::Clock clock;

    std::list<Vec2i> snek;
    std::queue<dir> keybuf;

    Vec2i apple;

    dir facing;

    bool ate_apple;

    void set_apple()
    {
        // continue to generate random positions until the apple isn't in the snake
        do {
            apple.x = rand() % BOARD_SIZE;
            apple.y = rand() % BOARD_SIZE;
        } while (std::find(snek.begin(), snek.end(), apple) != snek.end());
    }

    void update()
    {
        if (keybuf.size())
        {
            facing = keybuf.front();
            keybuf.pop();
        }

        // add front of snake ("moving" forwards)
        Vec2i front_pos = snek.front() + vec_dirs[facing];

        if (!ate_apple)
            // remove last element (end of snake)
            snek.pop_back();
        else
            ate_apple = false;

        // crash into snake/wall
        if (std::find(snek.begin(), snek.end(), front_pos) != snek.end() ||
                front_pos.x < 0 ||
                front_pos.x >= BOARD_SIZE ||
                front_pos.y < 0 ||
                front_pos.y >= BOARD_SIZE)
            game_state = state::LOST;
        else
            snek.push_front(front_pos);
        
        if (snek.front() == apple)
        {
            ate_apple = true;
            set_apple();
        }
    }

    bool key_valid(dir curdir, dir keydir)
    {
        return curdir != keydir && (curdir & 1) != (keydir & 1);
    }

    void handle_keydir(dir keydir)
    {
        // most likely state
        if (game_state == state::PLAYING)
        {
            // buffer is empty
            if (keybuf.size() == 0 && key_valid(facing, keydir))
                keybuf.push(keydir);
            // not the current direction and not facing backwards
            else if (keybuf.size() < MAX_KEYPRESSES && keybuf.size() < MAX_KEYPRESSES && key_valid(keybuf.back(), keydir))
                keybuf.push(keydir);
        }
        else if (game_state == state::START)
        {
            facing = keydir;
            game_state = state::PLAYING;
        }
        else if (game_state == state::LOST)
        {
            game_state = state::START;

            // reset snake
            snek = { START_POS };
        }
    }

public:
    Game(int milli_cycle)
        : milli_update(milli_cycle)
        , game_state(state::START)
        , win(sf::VideoMode(400, 400), "snek", sf::Style::Titlebar)
        , view(sf::Vector2f(BOARD_SIZE/2.f, BOARD_SIZE/2.f), sf::Vector2f(BOARD_SIZE, BOARD_SIZE))
        , snek({ START_POS })
        , facing(INVALID)
        , ate_apple(false)
    {
        win.setView(view);

        set_apple();
    }

    void run()
    {
        while (win.isOpen())
        {
            sf::Event event;
            while (win.pollEvent(event))
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    switch (event.key.code)
                    {
                        case sf::Keyboard::W:
                            handle_keydir(dir::UP);
                            break;
                        case sf::Keyboard::A:
                            handle_keydir(dir::LEFT);
                            break;
                        case sf::Keyboard::S:
                            handle_keydir(dir::DOWN);
                            break;
                        case sf::Keyboard::D:
                            handle_keydir(dir::RIGHT);
                            break;
                        default: break;
                    }
                }
                else if (event.type == sf::Event::Closed)
                    win.close();
            }

            win.clear();

            // most likely state
            if (game_state == state::PLAYING)
            {
                if (clock.getElapsedTime().asMilliseconds() >= milli_update)
                {
                    clock.restart();
                    update();
                }
            }
            else if (game_state == state::LOST)
            {
                sf::RectangleShape r(sf::Vector2f(3, 3));
                r.setFillColor(sf::Color(100, 100, 100));
                r.setPosition(sf::Vector2f(7, 7));
                win.draw(r);
            }

            // ----- DRAW SQUARES ----- //
            sf::RectangleShape r(sf::Vector2f(0.8f, 0.8f));
            for (auto &p : snek)
            {
                r.setPosition(p.to_sf_vec() + sf::Vector2f(0.1f, 0.1f));
                win.draw(r);
            }

            r.setFillColor(sf::Color::Red);
            r.setPosition(sf::Vector2f(apple.to_sf_vec() + sf::Vector2f(0.1f, 0.1f)));
            win.draw(r);
            // ----- ------------ ----- //

            win.display();
        }
    }
};

const Vec2i Game::vec_dirs[5] = {
    Vec2i(0, 0),
    Vec2i(0, -1),
    Vec2i(-1, 0),
    Vec2i(0, 1),
    Vec2i(1, 0),
};

const Vec2i Game::START_POS(3, 7);

int main()
{
    Game g(150);
    g.run();
}
