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

    ChessPiece::PieceColor currentPlayingColor = ChessPiece::PieceColor::White;

    bool pieceIsSelected = false;
    sf::Vector2u selectedPiecePosition;
    std::vector<sf::RectangleShape> movement_squares;

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

                    if (pieceIsSelected)
                    {
                        std::vector<Move> moves = board.GetPieceAt(selectedPiecePosition)->GetAvailableMoves();
                        // Filter moves here, taking into account pining and stuff.
                        for (auto move = moves.begin(); move != moves.end(); ++move)
                        {
                            sf::Vector2u pos = {(uint8_t)boardPosition.x,
                                                (uint8_t)boardPosition.y};
                            if (move->destination == pos && pieceIsSelected)
                            {
                                board.PerformMove(*move);
                                // Check for mates here.
                                currentPlayingColor = currentPlayingColor == ChessPiece::PieceColor::White ? ChessPiece::PieceColor::Black : ChessPiece::PieceColor::White;
                                break;
                            }
                        }
                        pieceIsSelected = false;
                        movement_squares.clear();
                    }
                    if (piece.has_value() && currentPlayingColor == piece->GetColor())
                    {
                        pieceIsSelected = true;
                        selectedPiecePosition = {(uint8_t)boardPosition.x, (uint8_t)boardPosition.y};
                        movement_squares.clear();
                        std::vector<Move> moves = piece->GetAvailableMoves();

                        for (auto move = moves.begin(); move != moves.end(); ++move)
                        {
                            auto rect = sf::RectangleShape({CELL_SIZE, CELL_SIZE});
                            rect.setPosition({(float)move->destination.x * CELL_SIZE, (float)move->destination.y * CELL_SIZE});
                            rect.setFillColor(sf::Color(0, 172, 0, 64));
                            movement_squares.push_back(rect);
                        }
                    }
                }
            }
        }

        window.clear();
        board.Draw(window);
        for (auto rect = movement_squares.begin(); rect != movement_squares.end(); ++rect)
        {
            window.draw(*rect);
        }
        // All drawing must be done between board.Draw and display.
        window.display();
    }
}
