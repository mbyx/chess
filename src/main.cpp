#include <SFML/Graphics.hpp>
#include <iostream>

#include "chessboard.h"
#include "asset_manager.h"

int main()
{
    sf::Vector2u windowSize = {CELL_SIZE * ROW_COUNT, CELL_SIZE * ROW_COUNT};

    auto window = sf::RenderWindow(sf::VideoMode(windowSize), "Chess", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    AssetManager manager;

    ChessBoard board(manager.GetBoardTexture(), manager.GetPieceTexture());

    bool pieceIsSelected = false;
    sf::Vector2u selectedPiecePosition;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    sf::Vector2i boardPosition = mousePosition / (int)CELL_SIZE;
                    auto &piece = board.GetPieceAt({(uint8_t)boardPosition.x, (uint8_t)boardPosition.y});
                    if (piece.has_value())
                    {
                        pieceIsSelected = true;
                        selectedPiecePosition = {(uint8_t)boardPosition.x, (uint8_t)boardPosition.y};
                    }
                    else
                    {
                        pieceIsSelected = false;
                    }
                }
                std::cout << pieceIsSelected << std::endl;
            }
        }

        window.clear();
        board.Draw(window);
        // All drawing must be done between board.Draw and display.
        window.display();
    }
}
