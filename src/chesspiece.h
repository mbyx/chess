#include <SFML/Graphics.hpp>
#include <string>

#define PIECE_SIZE 16u
#define SPRITE_SCALING 4u

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
        Black,
        White,
    };

    void Draw(sf::RenderWindow &window)
    {
        window.draw(m_Sprite);
    }

    ChessPiece(PieceType type, PieceColor color, sf::Texture &texture);

    sf::Sprite &GetSprite()
    {
        return m_Sprite;
    }

private:
    PieceType m_Type;
    PieceColor m_Color;

    sf::Vector2u m_Coords;
    sf::Sprite m_Sprite;
};