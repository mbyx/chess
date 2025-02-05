#include <SFML/Graphics.hpp>

#include "chessboard.h"
#include <iostream>

ChessBoard::ChessBoard(sf::Texture &boardTexture, sf::Texture &pieceTexture) : m_Sprite(boardTexture)
{
    m_Sprite.setScale({SPRITE_SCALING, SPRITE_SCALING});

    ConstructFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", pieceTexture);

    for (size_t row = 0; row != ROW_COUNT; ++row)
    {
        for (size_t column = 0; column != ROW_COUNT; ++column)
        {
            auto &piece = m_Pieces[row][column];
            if (piece.has_value())
            {
                sf::Vector2f position = {static_cast<float>(column * PIECE_SIZE * SPRITE_SCALING),
                                         static_cast<float>(row * PIECE_SIZE * SPRITE_SCALING)};
                piece->GetSprite().setPosition(position);
            }
        }
    }
}

// https://www.chess.com/terms/fen-chess
void ChessBoard::ConstructFromFEN(std::string fen, sf::Texture &pieceTexture)
{
    // For now we only parse the first field of FEN that defines the board layout.
    uint8_t currentRow = 0;
    uint8_t currentColumn = 0;
    for (auto character = fen.begin(); character != fen.end(); ++character)
    {
        if (*character == '/')
        {
            currentRow++;
            currentColumn = 0;
        }
        else if (std::isdigit(*character))
        {
            // Convert character digit to an integer.
            uint8_t space = *character - '0';
            currentColumn += space;
        }
        else
        {
            ChessPiece::PieceColor color = std::islower(*character) ? ChessPiece::PieceColor::Black
                                                                    : ChessPiece::PieceColor::White;

            ChessPiece::PieceType type;
            switch (std::tolower(*character))
            {
            case 'p':
                type = ChessPiece::PieceType::Pawn;
                break;
            case 'r':
                type = ChessPiece::PieceType::Rook;
                break;
            case 'n':
                type = ChessPiece::PieceType::Knight;
                break;
            case 'b':
                type = ChessPiece::PieceType::Bishop;
                break;
            case 'q':
                type = ChessPiece::PieceType::Queen;
                break;
            case 'k':
                type = ChessPiece::PieceType::King;
                break;
            }

            m_Pieces[currentRow][currentColumn] = ChessPiece(type, color, pieceTexture);
            currentColumn++;
        }
    }
}

void ChessBoard::Draw(sf::RenderWindow &window)
{
    window.draw(m_Sprite);
    for (size_t row = 0; row != ROW_COUNT; ++row)
    {
        for (size_t column = 0; column != ROW_COUNT; ++column)
        {
            std::optional<ChessPiece> piece = m_Pieces[row][column];
            if (piece.has_value())
                piece.value().Draw(window);
        }
    }
}

std::vector<ChessPiece> ChessBoard::GetPiecesByType(ChessPiece::PieceType type)
{
    std::vector<ChessPiece> pieces;

    for (size_t row = 0; row != ROW_COUNT; ++row)
    {
        for (size_t column = 0; column != ROW_COUNT; ++column)
        {
            if (m_Pieces[row][column].has_value())
            {
                ChessPiece piece = m_Pieces[row][column].value();
                if (piece.GetType() == type)
                    pieces.push_back(piece);
            }
        }
    }
    return pieces;
}

std::vector<ChessPiece> ChessBoard::GetPiecesByColor(ChessPiece::PieceColor color)
{
    std::vector<ChessPiece> pieces;

    for (size_t row = 0; row != ROW_COUNT; ++row)
    {
        for (size_t column = 0; column != ROW_COUNT; ++column)
        {
            if (m_Pieces[row][column].has_value())
            {
                ChessPiece piece = m_Pieces[row][column].value();
                if (piece.GetColor() == color)
                    pieces.push_back(piece);
            }
        }
    }
    return pieces;
}

std::optional<ChessPiece> &ChessBoard::GetPieceAt(sf::Vector2u position)
{
    return m_Pieces[position.y][position.x];
}

std::vector<Move> ChessBoard::GetPseudoLegalMoves(ChessPiece::PieceColor currentPlayingColor, sf::Vector2u pieceAtPosition)
{
    std::vector<Move> moves = GetPieceAt(pieceAtPosition)->GetAvailableMoves();
    std::vector<sf::Vector2u> blockedDirs;

    // For each direction, we go across it until the end (farther than it actually, but we skip invalid ones).
    // Until there is a piece we don't do anything, after the piece, we mark all squares after that to be blocking.
    // Since this checks every direction regardless of piece, it is inefficient.
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
                    static_cast<uint8_t>(pieceAtPosition.x + w * i),
                    static_cast<uint8_t>(pieceAtPosition.y + w * j)};

                if (pos.x >= ROW_COUNT || pos.y >= ROW_COUNT)
                    break;

                if (blocking)
                {
                    blockedDirs.push_back(pos);
                    continue;
                }

                auto piece = GetPieceAt(pos);
                if (piece.has_value())
                {
                    if (piece->GetColor() == currentPlayingColor)
                        blockedDirs.push_back(pos);
                    blocking = true;
                }
            }
        }
    }

    // We go through every move and check if that move is blocked, and if so, do not add it.
    // On the other hand, we also need to filter our special cases. The knight can jump over
    // other pieces, so all it needs to check is if it's jumping destination has an ally piece.
    // The pawn, because of how it's been programmed also has the moves for taking left and right.
    // So we simply filter them out if there isn't an enemy piece to the sides. We also only allow
    // Moving two steps the first time it moves.
    std::vector<Move> pseudoLegalMoves;
    for (auto move = moves.begin(); move != moves.end(); ++move)
    {

        if (GetPieceAt(pieceAtPosition)->GetType() == ChessPiece::PieceType::Knight)
        {
            auto &piece = GetPieceAt(move->destination);
            if (piece.has_value() && piece->GetColor() == currentPlayingColor)
                continue;
        }
        if (GetPieceAt(pieceAtPosition)->GetType() == ChessPiece::PieceType::Pawn)
        {
            auto &destPiece = GetPieceAt(move->destination);
            if (move->source.x != move->destination.x && !destPiece.has_value())
                continue;
            if (abs((int)(move->source.y - move->destination.y)) == 2 && GetPieceAt(pieceAtPosition)->GetMoveCount() >= 1)
                continue;
            if (move->source.x == move->destination.x && destPiece.has_value())
                continue;
        }
        if (std::find(blockedDirs.begin(), blockedDirs.end(), move->destination) != blockedDirs.end())
        {
            continue;
        }
        pseudoLegalMoves.push_back(*move);
    }

    return pseudoLegalMoves;
}

std::vector<Move> ChessBoard::GetLegalMoves(ChessPiece::PieceColor currentPlayingColor, sf::Vector2u pieceAtPosition)
{
    // Here we basically get the moves of every piece of the current color, and try them out.
    // Then we check if the opposing color has any future move that results in the king being taken.
    // If so, then that move would be invalid for the current color. So we then undo that move.
    // This is very slow, but because of the board size, it is not problematic.
    std::vector<Move> legalMoves;

    std::vector<Move> moves = GetPseudoLegalMoves(currentPlayingColor, pieceAtPosition);

    for (auto move = moves.begin(); move != moves.end(); ++move)
    {
        PerformMove(*move);
        bool pinned = false;
        auto opposingColor = currentPlayingColor == ChessPiece::PieceColor::White ? ChessPiece::PieceColor::Black : ChessPiece::PieceColor::White;
        auto pieces = GetPiecesByColor(opposingColor);
        for (auto &piece = pieces.begin(); piece != pieces.end(); piece++)
        {
            auto pos2 = piece->GetSprite().getPosition();
            auto pseudoLegalMoves = GetPseudoLegalMoves(opposingColor, {(uint16_t)pos2.x / CELL_SIZE, (uint16_t)pos2.y / CELL_SIZE});
            for (auto pseudoLegalMove = pseudoLegalMoves.begin(); pseudoLegalMove != pseudoLegalMoves.end(); pseudoLegalMove++)
            {
                auto &takingPiece = GetPieceAt(pseudoLegalMove->destination);
                if (takingPiece.has_value() && takingPiece->GetType() == ChessPiece::PieceType::King && takingPiece->GetColor() == currentPlayingColor)
                    pinned = true;
            }
        }
        UnPerformMove(*move);
        if (pinned)
            continue;

        legalMoves.push_back(*move);
    }
    return legalMoves;
}

bool ChessBoard::IsInCheckMate(ChessPiece::PieceColor currentPlayingColor)
{
    auto pieces = GetPiecesByColor(currentPlayingColor);
    int numberOfMoves = 0;
    for (auto piece = pieces.begin(); piece != pieces.end(); ++piece)
    {
        auto spritePosition = piece->GetSprite().getPosition();
        sf::Vector2u pieceAtPosition = {(uint16_t)spritePosition.x / CELL_SIZE,
                                        (uint16_t)spritePosition.y / CELL_SIZE};
        numberOfMoves += GetLegalMoves(currentPlayingColor, pieceAtPosition).size();
    }
    return numberOfMoves == 0;
}

void ChessBoard::PerformMove(Move move)
{
    auto &piece = GetPieceAt(move.source);
    piece->SetMoveCount(piece->GetMoveCount() + 1);
    piece->GetSprite()
        .setPosition({static_cast<float>(move.destination.x * CELL_SIZE), static_cast<float>(move.destination.y * CELL_SIZE)});
    m_LastTakenPiece = m_Pieces[move.destination.y][move.destination.x];
    m_Pieces[move.destination.y][move.destination.x] = piece;
    m_Pieces[move.source.y][move.source.x].reset();
}

void ChessBoard::UnPerformMove(Move move)
{
    auto &piece = GetPieceAt(move.destination);
    piece->SetMoveCount(piece->GetMoveCount() - 1);
    piece->GetSprite()
        .setPosition({static_cast<float>(move.source.x * CELL_SIZE), static_cast<float>(move.source.y * CELL_SIZE)});
    m_Pieces[move.source.y][move.source.x] = piece;
    m_Pieces[move.destination.y][move.destination.x] = m_LastTakenPiece;
    // TODO: Allow unperforming more than one move.
    // Right now this line assumes that no other move was made before this, which
    // only works for one move. It isn't a problem as we only make one move before
    // un moving.
    // One way to potentially solve this would be to store a vector of taken pieces,
    // sorted by recency, then the last element will be the last piece taken and can
    // simply be popped.
    m_LastTakenPiece.reset();
}
