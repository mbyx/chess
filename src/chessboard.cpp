#include <SFML/Graphics.hpp>

#include "chessboard.h"

ChessBoard::ChessBoard()
{
    for (size_t row = 0; row != ROW_COUNT; ++row)
    {
        for (size_t column = 0; column != ROW_COUNT; ++column)
        {
            rects[row][column] = sf::RectangleShape({CELL_SIZE, CELL_SIZE});

            rects[row][column].setPosition({(float)row * CELL_SIZE, (float)column * CELL_SIZE});
            rects[row][column].setFillColor((row + column) % 2 == 0 ? LIGHT_COLOR : DARK_COLOR);
        }
    }
}

void ChessBoard::Draw(sf::RenderWindow &window)
{
    for (size_t i = 0; i != ROW_COUNT; ++i)
    {
        for (size_t j = 0; j != ROW_COUNT; ++j)
        {
            window.draw(rects[i][j]);
        }
    }
}