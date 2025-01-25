#include "chesspiece.h"

ChessPiece::ChessPiece(PieceType type, PieceColor color, sf::Texture &texture) : m_Type(type), m_Color(color), m_Sprite(texture)
{
    switch (m_Type)
    {
    case PieceType::Pawn:
        m_TileCoords = color == PieceColor::Black ? sf::Vector2u(0, 0) : sf::Vector2u(0, 1);
        break;
    case PieceType::Rook:
        m_TileCoords = color == PieceColor::Black ? sf::Vector2u(1, 0) : sf::Vector2u(1, 1);
        break;
    case PieceType::Bishop:
        m_TileCoords = color == PieceColor::Black ? sf::Vector2u(2, 0) : sf::Vector2u(2, 1);
        break;
    case PieceType::Knight:
        m_TileCoords = color == PieceColor::Black ? sf::Vector2u(3, 0) : sf::Vector2u(3, 1);
        break;
    case PieceType::Queen:
        m_TileCoords = color == PieceColor::Black ? sf::Vector2u(4, 0) : sf::Vector2u(4, 1);
        break;
    case PieceType::King:
        m_TileCoords = color == PieceColor::Black ? sf::Vector2u(5, 0) : sf::Vector2u(5, 1);
        break;
    }
    m_Sprite.setTextureRect({{(int)m_TileCoords.x * (int)PIECE_SIZE, (int)m_TileCoords.y * (int)PIECE_SIZE}, {(int)PIECE_SIZE, (int)PIECE_SIZE}});
    m_Sprite.setScale({SPRITE_SCALING, SPRITE_SCALING});
}

std::vector<Move> ChessPiece::GetAvailableMoves()
{
    sf::Vector2f source = m_Sprite.getPosition() / (float)CELL_SIZE;
    std::vector<Move> moves;
    switch (m_Type)
    {
    case PieceType::Pawn:
        // weirdest one.
        break;
    case PieceType::Rook:
        for (uint8_t column = 0; column != ROW_COUNT; ++column)
        {
            if (column != source.x)
            {
                Move move;
                move.source = {(uint8_t)source.x, (uint8_t)source.y};
                move.destination = {column, (uint8_t)source.y};
                moves.push_back(move);
            }
        }
        for (uint8_t row = 0; row != ROW_COUNT; ++row)
        {
            if (row != source.y)
            {
                Move move;
                move.source = {(uint8_t)source.x, (uint8_t)source.y};
                move.destination = {(uint8_t)source.x, row};
                moves.push_back(move);
            }
        }
        break;
    case PieceType::Bishop:
        break;
    case PieceType::Knight:
        break;
    case PieceType::Queen:
        for (uint8_t column = 0; column != ROW_COUNT; ++column)
        {
            if (column != source.x)
            {
                Move move;
                move.source = {(uint8_t)source.x, (uint8_t)source.y};
                move.destination = {column, (uint8_t)source.y};
                moves.push_back(move);
            }
        }
        for (uint8_t row = 0; row != ROW_COUNT; ++row)
        {
            if (row != source.y)
            {
                Move move;
                move.source = {(uint8_t)source.x, (uint8_t)source.y};
                move.destination = {(uint8_t)source.x, row};
                moves.push_back(move);
            }
        }
        break;
    case PieceType::King:
        break;
    }
    return moves;
}
