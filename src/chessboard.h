#include <SFML/Graphics.hpp>
#include "chesspiece.h"

#define SPRITE_SCALING 4u
#define PIECE_SIZE 16u
#define ROW_COUNT 8u
#define CELL_SIZE (PIECE_SIZE * SPRITE_SCALING)

class ChessBoard
{
public:
    ChessBoard(sf::Texture &texture, sf::Texture &pieceTexture);

    void Draw(sf::RenderWindow &window);

private:
    std::optional<ChessPiece> m_Pieces[ROW_COUNT][ROW_COUNT];
    sf::Sprite m_Sprite;
};