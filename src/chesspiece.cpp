#include "chesspiece.h"

ChessPiece::ChessPiece(PieceType type, PieceColor color, sf::Texture &texture) : m_Type(type), m_Color(color), m_Sprite(texture)
{
    switch (m_Type)
    {
    case PieceType::Pawn:
        m_Coords = color == PieceColor::White ? sf::Vector2u(0, 0) : sf::Vector2u(0, 1);
        break;
    case PieceType::Rook:
        m_Coords = color == PieceColor::White ? sf::Vector2u(1, 0) : sf::Vector2u(1, 1);
        break;
    case PieceType::Bishop:
        m_Coords = color == PieceColor::White ? sf::Vector2u(2, 0) : sf::Vector2u(2, 1);
        break;
    case PieceType::Knight:
        m_Coords = color == PieceColor::White ? sf::Vector2u(3, 0) : sf::Vector2u(3, 1);
        break;
    case PieceType::Queen:
        m_Coords = color == PieceColor::White ? sf::Vector2u(4, 0) : sf::Vector2u(4, 1);
        break;
    case PieceType::King:
        m_Coords = color == PieceColor::White ? sf::Vector2u(5, 0) : sf::Vector2u(5, 1);
        break;
    }
    m_Sprite.setTextureRect({{(int)m_Coords.x * (int)PIECE_SIZE, (int)m_Coords.y * (int)PIECE_SIZE}, {(int)PIECE_SIZE, (int)PIECE_SIZE}});
    m_Sprite.setScale({SPRITE_SCALING, SPRITE_SCALING});
}