#include "chesspiece.h"
#include <iostream>

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
    {
        Move move;
        move.source = {(uint8_t)source.x, (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x), (uint8_t)(source.y + (m_Color == PieceColor::White ? -1 : 1))};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x, (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x), (uint8_t)(source.y + (m_Color == PieceColor::White ? -2 : 2))};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x, (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x + 1), (uint8_t)(source.y + (m_Color == PieceColor::White ? -1 : 1))};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x, (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x - 1), (uint8_t)(source.y + (m_Color == PieceColor::White ? -1 : 1))};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);
    }
    break;
    case PieceType::Rook:
    {
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
    }
    break;
    case PieceType::Bishop:
    {
        uint8_t dist_x_end = ROW_COUNT - source.x - 1;
        uint8_t dist_x_start = source.x;
        uint8_t dist_y_end = ROW_COUNT - source.y - 1;
        uint8_t dist_y_start = source.y;

        uint8_t primary_axis_start = std::min(dist_x_start, dist_y_start);
        uint8_t primary_axis_end = std::min(dist_x_end, dist_y_end);

        for (int i = -primary_axis_start; i <= primary_axis_end; ++i)
        {
            if (i == 0)
                continue;
            Move move;
            move.source = {(uint8_t)source.x,
                           (uint8_t)source.y};
            move.destination = {(uint8_t)(source.x + i), (uint8_t)(source.y + i)};
            moves.push_back(move);
        }

        uint8_t secondary_axis_start = std::min(dist_x_start, dist_y_end);
        uint8_t secondary_axis_end = std::min(dist_x_end, dist_y_start);

        for (int i = -secondary_axis_start; i <= secondary_axis_end; ++i)
        {
            if (i == 0)
                continue;
            Move move;
            move.source = {(uint8_t)source.x,
                           (uint8_t)source.y};
            move.destination = {(uint8_t)(source.x + i), (uint8_t)(source.y - i)};
            moves.push_back(move);
        }
    }

    break;
    case PieceType::Knight:
    {
        Move move;
        move.source = {(uint8_t)source.x,
                       (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x + 2), (uint8_t)(source.y + 1)};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x,
                       (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x + 1), (uint8_t)(source.y + 2)};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x,
                       (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x - 2), (uint8_t)(source.y - 1)};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x,
                       (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x - 1), (uint8_t)(source.y - 2)};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x,
                       (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x - 2), (uint8_t)(source.y + 1)};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x,
                       (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x - 1), (uint8_t)(source.y + 2)};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x,
                       (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x + 2), (uint8_t)(source.y - 1)};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x,
                       (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x + 1), (uint8_t)(source.y - 2)};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);
    }
    break;
    case PieceType::Queen:
    {
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

        uint8_t dist_x_end = ROW_COUNT - source.x - 1;
        uint8_t dist_x_start = source.x;
        uint8_t dist_y_end = ROW_COUNT - source.y - 1;
        uint8_t dist_y_start = source.y;

        uint8_t primary_axis_start = std::min(dist_x_start, dist_y_start);
        uint8_t primary_axis_end = std::min(dist_x_end, dist_y_end);

        for (int i = -primary_axis_start; i <= primary_axis_end; ++i)
        {
            if (i == 0)
                continue;
            Move move;
            move.source = {(uint8_t)source.x,
                           (uint8_t)source.y};
            move.destination = {(uint8_t)(source.x + i), (uint8_t)(source.y + i)};
            moves.push_back(move);
        }

        uint8_t secondary_axis_start = std::min(dist_x_start, dist_y_end);
        uint8_t secondary_axis_end = std::min(dist_x_end, dist_y_start);

        for (int i = -secondary_axis_start; i <= secondary_axis_end; ++i)
        {
            if (i == 0)
                continue;
            Move move;
            move.source = {(uint8_t)source.x,
                           (uint8_t)source.y};
            move.destination = {(uint8_t)(source.x + i), (uint8_t)(source.y - i)};
            moves.push_back(move);
        }
    }
    break;
    case PieceType::King:
    {
        Move move;
        move.source = {(uint8_t)source.x,
                       (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x + 1),
                            (uint8_t)(source.y + 0)};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x,
                       (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x + 0),
                            (uint8_t)(source.y + 1)};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x,
                       (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x + 1),
                            (uint8_t)(source.y + 1)};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x,
                       (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x - 1),
                            (uint8_t)(source.y + 0)};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x,
                       (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x + 0),
                            (uint8_t)(source.y - 1)};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x,
                       (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x - 1),
                            (uint8_t)(source.y - 1)};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x,
                       (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x + 1),
                            (uint8_t)(source.y - 1)};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);

        move.source = {(uint8_t)source.x,
                       (uint8_t)source.y};
        move.destination = {(uint8_t)(source.x - 1),
                            (uint8_t)(source.y + 1)};
        if (move.destination.x < ROW_COUNT && move.destination.y < ROW_COUNT)
            moves.push_back(move);
    }
    break;
    }
    return moves;
}
