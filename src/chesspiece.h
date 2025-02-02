#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <algorithm>

#define SPRITE_SCALING 4u
#define PIECE_SIZE 16u
#define ROW_COUNT 8u
#define CELL_SIZE (PIECE_SIZE * SPRITE_SCALING)

struct Move
{
    // In board position coordinates.
    sf::Vector2u source;
    sf::Vector2u destination;
};

class ChessPiece
{
public:
    // Note: Order does matter as these are used as the index into a spritesheet.
    enum PieceType
    {
        Pawn,
        Rook,
        Bishop,
        Knight,
        King,
        Queen,
    };

    // Note: Order does matter as these are used as the index into a spritesheet.
    enum PieceColor
    {
        White,
        Black,
    };

    ChessPiece(PieceType type, PieceColor color, sf::Texture &texture);

    std::vector<Move> GetAvailableMoves();

    void Draw(sf::RenderWindow &window)
    {
        window.draw(m_Sprite);
    }

    sf::Sprite &GetSprite()
    {
        return m_Sprite;
    }

    PieceType GetType()
    {
        return m_Type;
    }

    PieceColor GetColor()
    {
        return m_Color;
    }

    int GetMoveCount()
    {
        return m_NumberOfMoves;
    }

    void SetMoveCount(int value)
    {
        m_NumberOfMoves = value;
    }

private:
    PieceType m_Type;
    PieceColor m_Color;
    int m_NumberOfMoves = 0;

    sf::Vector2u m_TileCoords;
    sf::Sprite m_Sprite;
};