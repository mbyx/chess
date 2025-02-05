#include <SFML/Graphics.hpp>

#include "chessboard/chessboard.h"
#include "asset_manager.h"

// Currently pawn promotion, en passant, and castling are not implemented.

int main()
{
    sf::Vector2u windowSize = {CELL_SIZE * ROW_COUNT, CELL_SIZE * ROW_COUNT};

    auto window = sf::RenderWindow(sf::VideoMode(windowSize), "Chess", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

start:
    AssetManager manager;

    ChessBoard board(manager.GetBoardTexture(), manager.GetPieceTexture());

    ChessPiece::PieceColor currentPlayingColor = ChessPiece::PieceColor::White;

    bool pieceIsSelected = false;
    sf::Vector2u selectedPiecePosition;
    std::vector<sf::RectangleShape> movementSquares;

    sf::Text checkMateText(manager.GetFont());
    checkMateText.setCharacterSize(48);
    checkMateText.setFillColor(sf::Color::Green);
    checkMateText.setOutlineThickness(2.0);
    checkMateText.setStyle(sf::Text::Bold);
    checkMateText.setPosition({windowSize.x / 2.0f, windowSize.y / 2.0f});
    checkMateText.move({-100, -48});

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (event->is<sf::Event::KeyPressed>())
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
                    window.close();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
                {
                    // Yes, I know goto is bad and all but in this case it works better.
                    goto start;
                }
            }

            if (event->is<sf::Event::MouseButtonPressed>() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                sf::Vector2i boardPosition = mousePosition / (int)CELL_SIZE;
                sf::Vector2u squareClicked = {(uint8_t)boardPosition.x,
                                              (uint8_t)boardPosition.y};
                auto &piece = board.GetPieceAt(squareClicked);
                if (pieceIsSelected)
                {
                    std::vector<Move> moves = board.GetLegalMoves(currentPlayingColor, selectedPiecePosition);

                    for (auto move = moves.begin(); move != moves.end(); ++move)
                    {
                        if (move->destination == squareClicked && pieceIsSelected)
                        {
                            board.PerformMove(*move);
                            currentPlayingColor = currentPlayingColor == ChessPiece::PieceColor::White
                                                      ? ChessPiece::PieceColor::Black
                                                      : ChessPiece::PieceColor::White;

                            if (board.IsInCheckMate(currentPlayingColor))
                            {
                                checkMateText.setString("Checkmate!");
                            }

                            break;
                        }
                    }
                    pieceIsSelected = false;
                    movementSquares.clear();
                }
                if (piece.has_value() && currentPlayingColor == piece->GetColor())
                {
                    pieceIsSelected = true;
                    selectedPiecePosition = {(uint8_t)boardPosition.x, (uint8_t)boardPosition.y};
                    movementSquares.clear();

                    std::vector<Move> moves = board.GetLegalMoves(currentPlayingColor, selectedPiecePosition);
                    for (auto move = moves.begin(); move != moves.end(); ++move)
                    {
                        auto rect = sf::RectangleShape({CELL_SIZE, CELL_SIZE});
                        rect.setPosition({(float)move->destination.x * CELL_SIZE, (float)move->destination.y * CELL_SIZE});
                        rect.setFillColor(sf::Color(0, 172, 0, 64));
                        movementSquares.push_back(rect);
                    }
                }
            }
        }

        window.clear();
        board.Draw(window);
        window.draw(checkMateText);
        if (pieceIsSelected)
        {
            auto rect = sf::RectangleShape({CELL_SIZE, CELL_SIZE});
            rect.setPosition(
                {static_cast<float>(selectedPiecePosition.x * CELL_SIZE),
                 static_cast<float>(selectedPiecePosition.y * CELL_SIZE)});
            rect.setFillColor(sf::Color(172, 172, 0, 64));
            window.draw(rect);
        }

        for (auto rect = movementSquares.begin(); rect != movementSquares.end(); ++rect)
        {
            window.draw(*rect);
        }

        window.display();
    }
}