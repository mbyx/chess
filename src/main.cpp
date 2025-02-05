#include <SFML/Graphics.hpp>
#include <iostream>

#include "chessboard/chessboard.h"
#include "asset_manager.h"

std::vector<Move> GetFilteredMoves(ChessPiece::PieceColor currentPlayingColor, sf::Vector2u selectedPiecePosition, ChessBoard &board)
{
    std::vector<Move> moves = board.GetPieceAt(selectedPiecePosition)->GetAvailableMoves();
    std::vector<sf::Vector2u> blockedDirs;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;

            bool blocking = false;

            for (int w = 1; w < ROW_COUNT; w++)
            {
                sf::Vector2u pos = {
                    static_cast<uint8_t>(selectedPiecePosition.x + w * i),
                    static_cast<uint8_t>(selectedPiecePosition.y + w * j)};

                if (pos.x >= ROW_COUNT || pos.y >= ROW_COUNT)
                    break;

                if (blocking)
                {
                    blockedDirs.push_back(pos);
                    continue;
                }

                auto piece = board.GetPieceAt(pos);
                if (piece.has_value())
                {
                    if (piece->GetColor() == currentPlayingColor)
                        blockedDirs.push_back(pos);
                    blocking = true;
                }
            }
        }
    }

    std::vector<Move> filteredMoves;
    for (auto move = moves.begin(); move != moves.end(); ++move)
    {

        if (board.GetPieceAt(selectedPiecePosition)->GetType() == ChessPiece::PieceType::Knight)
        {
            auto &piece = board.GetPieceAt(move->destination);
            if (piece.has_value() && piece->GetColor() == currentPlayingColor)
                continue;
        }
        if (board.GetPieceAt(selectedPiecePosition)->GetType() == ChessPiece::PieceType::Pawn)
        {
            auto &destPiece = board.GetPieceAt(move->destination);
            if (move->source.x != move->destination.x && !destPiece.has_value())
                continue;
            if (abs((int)(move->source.y - move->destination.y)) == 2 && board.GetPieceAt(selectedPiecePosition)->GetMoveCount() >= 1)
                continue;
            if (move->source.x == move->destination.x && destPiece.has_value())
                continue;
        }
        if (std::find(blockedDirs.begin(), blockedDirs.end(), move->destination) != blockedDirs.end())
        {
            continue;
        }
        filteredMoves.push_back(*move);
    }

    return filteredMoves;
}

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
                    sf::Vector2u pos = {(uint8_t)boardPosition.x,
                                        (uint8_t)boardPosition.y};
                    if (pieceIsSelected)
                    {
                        std::vector<Move> moves = GetFilteredMoves(currentPlayingColor, selectedPiecePosition, board);

                        for (auto move = moves.begin(); move != moves.end(); ++move)
                        {
                            board.PerformMove(*move);
                            bool pinned = false;
                            auto opposingColor = currentPlayingColor == ChessPiece::PieceColor::White ? ChessPiece::PieceColor::Black : ChessPiece::PieceColor::White;
                            auto pieces = board.GetPiecesByColor(opposingColor);
                            for (auto &piece = pieces.begin(); piece != pieces.end(); piece++)
                            {
                                auto pos2 = piece->GetSprite().getPosition();
                                auto filteredMoves = GetFilteredMoves(opposingColor, {(uint16_t)pos2.x / CELL_SIZE, (uint16_t)pos2.y / CELL_SIZE}, board);
                                for (auto filteredMove = filteredMoves.begin(); filteredMove != filteredMoves.end(); filteredMove++)
                                {
                                    auto &takingPiece = board.GetPieceAt(filteredMove->destination);
                                    if (takingPiece.has_value() && takingPiece->GetType() == ChessPiece::PieceType::King && takingPiece->GetColor() == currentPlayingColor)
                                        pinned = true;
                                }
                            }
                            board.UnPerformMove(*move);
                            if (pinned)
                                continue;

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
                        std::vector<Move> moves = GetFilteredMoves(currentPlayingColor, selectedPiecePosition, board);

                        for (auto move = moves.begin(); move != moves.end(); ++move)
                        {
                            board.PerformMove(*move);
                            bool pinned = false;
                            auto opposingColor = currentPlayingColor == ChessPiece::PieceColor::White ? ChessPiece::PieceColor::Black : ChessPiece::PieceColor::White;
                            auto pieces = board.GetPiecesByColor(opposingColor);
                            for (auto &piece = pieces.begin(); piece != pieces.end(); piece++)
                            {
                                auto pos2 = piece->GetSprite().getPosition();
                                auto filteredMoves = GetFilteredMoves(opposingColor, {(uint16_t)pos2.x / CELL_SIZE, (uint16_t)pos2.y / CELL_SIZE}, board);
                                for (auto filteredMove = filteredMoves.begin(); filteredMove != filteredMoves.end(); filteredMove++)
                                {
                                    auto &takingPiece = board.GetPieceAt(filteredMove->destination);
                                    if (takingPiece.has_value() && (takingPiece->GetType() == ChessPiece::PieceType::King && takingPiece->GetColor() == currentPlayingColor))
                                        pinned = true;
                                }
                            }
                            board.UnPerformMove(*move);
                            if (pinned)
                                continue;

                            auto rect = sf::RectangleShape({CELL_SIZE, CELL_SIZE});
                            rect.setPosition({(float)move->destination.x * CELL_SIZE, (float)move->destination.y * CELL_SIZE});
                            rect.setFillColor(sf::Color(0, 172, 0, 64));
                            movement_squares.push_back(rect);
                        }
                        // Selected piece highlight
                        auto rect = sf::RectangleShape({CELL_SIZE, CELL_SIZE});
                        rect.setPosition({(float)selectedPiecePosition.x * CELL_SIZE, (float)selectedPiecePosition.y * CELL_SIZE});
                        rect.setFillColor(sf::Color(172, 172, 0, 64));
                        movement_squares.push_back(rect);
                    }

                    bool isMoveAvailable = false;
                    auto pieces = board.GetPiecesByColor(currentPlayingColor);
                    for (auto &piece = pieces.begin(); piece != pieces.end(); piece++)
                    {
                        auto pos2 = piece->GetSprite().getPosition();
                        std::vector<Move> moves = GetFilteredMoves(currentPlayingColor, {(uint16_t)pos2.x / CELL_SIZE, (uint16_t)pos2.y / CELL_SIZE}, board);

                        for (auto move = moves.begin(); move != moves.end(); ++move)
                        {
                            board.PerformMove(*move);
                            bool pinned = false;
                            auto opposingColor = currentPlayingColor == ChessPiece::PieceColor::White ? ChessPiece::PieceColor::Black : ChessPiece::PieceColor::White;
                            auto pieces = board.GetPiecesByColor(opposingColor);
                            for (auto &piece = pieces.begin(); piece != pieces.end(); piece++)
                            {
                                auto pos2 = piece->GetSprite().getPosition();
                                auto filteredMoves = GetFilteredMoves(opposingColor, {(uint16_t)pos2.x / CELL_SIZE, (uint16_t)pos2.y / CELL_SIZE}, board);
                                for (auto filteredMove = filteredMoves.begin(); filteredMove != filteredMoves.end(); filteredMove++)
                                {
                                    auto &takingPiece = board.GetPieceAt(filteredMove->destination);
                                    if (takingPiece.has_value() && takingPiece->GetType() == ChessPiece::PieceType::King && takingPiece->GetColor() == currentPlayingColor)
                                        pinned = true;
                                }
                            }
                            board.UnPerformMove(*move);
                            if (pinned)
                                continue;

                            isMoveAvailable = true;
                        }
                    }
                    std::cout << currentPlayingColor << std::endl;
                    if (!isMoveAvailable)
                    {
                        std::cout << "Checkmate!" << std::endl;
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
