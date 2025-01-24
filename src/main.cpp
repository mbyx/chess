#include <SFML/Graphics.hpp>
#include <iostream>

#include "chessboard.h"

int main()
{
    sf::Vector2u windowSize = {CELL_SIZE * ROW_COUNT, CELL_SIZE * ROW_COUNT};

    auto window = sf::RenderWindow(sf::VideoMode(windowSize), "Chess", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // sf::Texture texture("../../../assets/black/rook.png");
    // sf::Sprite sprite(texture);
    // sprite.setScale({SPRITE_SCALING, SPRITE_SCALING});

    ChessBoard board;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();
        board.Draw(window);
        // All drawing must be done between board.Draw and display.

        // window.draw(sprite);
        window.display();
    }
}
