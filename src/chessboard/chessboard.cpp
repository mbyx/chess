#include <SFML/Graphics.hpp>

#include "chessboard.h"

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
