#pragma once

#include <algorithm>

#include <SFML/Graphics.hpp>

#include "../defines.h"

struct Move
{
    // NOTE: These use board position coordinates, so (1, 2) is the first column,
    // second row, or B6 (probably, I don't do chess).
    sf::Vector2u source;
    sf::Vector2u destination;
};

void AddBishopMoves(sf::Vector2f source, std::vector<Move> &moves);
void AddRookMoves(sf::Vector2f source, std::vector<Move> &moves);

class ChessPiece
{
public:
    enum PieceType
    {
        Pawn,
        Rook,
        Bishop,
        Knight,
        King,
        Queen,
    };

    enum PieceColor
    {
        White,
        Black,
    };

    ChessPiece(PieceType type, PieceColor color, sf::Texture &texture);

    std::vector<Move> GetAvailableMoves();

    void Draw(sf::RenderWindow &window) { window.draw(m_Sprite); }

    sf::Sprite &GetSprite() { return m_Sprite; }
    PieceType GetType() { return m_Type; }
    PieceColor GetColor() { return m_Color; }

    int GetMoveCount() { return m_NumberOfMoves; }
    void SetMoveCount(int value) { m_NumberOfMoves = value; }

private:
    PieceType m_Type;
    PieceColor m_Color;
    int m_NumberOfMoves = 0;

    sf::Vector2u m_TileCoords;
    sf::Sprite m_Sprite;
};