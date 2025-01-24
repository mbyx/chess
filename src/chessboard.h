#include <SFML/Graphics.hpp>

#define SPRITE_SCALING 4u
#define PIECE_SIZE 16u
#define ROW_COUNT 8u
#define CELL_SIZE (PIECE_SIZE * SPRITE_SCALING)

const sf::Color DARK_COLOR = sf::Color(0xb5, 0x88, 0x63);
const sf::Color LIGHT_COLOR = sf::Color(0xf0, 0xd9, 0xb5);

class ChessBoard
{
public:
    ChessBoard();

    void Draw(sf::RenderWindow &window);

private:
    sf::RectangleShape rects[ROW_COUNT][ROW_COUNT];
};