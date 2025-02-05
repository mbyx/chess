#include "chesspiece.h"

ChessPiece::ChessPiece(PieceType type, PieceColor color, sf::Texture &texture)
    : m_Type(type), m_Color(color), m_Sprite(texture)
{
    // This maps the piece type and color to the correct sprite in the sheet, using it's coordinates.
    switch (m_Type)
    {
    case PieceType::Pawn:
        m_TileCoords = (color == PieceColor::Black) ? sf::Vector2u(0, 0) : sf::Vector2u(0, 1);
        break;
    case PieceType::Rook:
        m_TileCoords = (color == PieceColor::Black) ? sf::Vector2u(1, 0) : sf::Vector2u(1, 1);
        break;
    case PieceType::Bishop:
        m_TileCoords = (color == PieceColor::Black) ? sf::Vector2u(2, 0) : sf::Vector2u(2, 1);
        break;
    case PieceType::Knight:
        m_TileCoords = (color == PieceColor::Black) ? sf::Vector2u(3, 0) : sf::Vector2u(3, 1);
        break;
    case PieceType::Queen:
        m_TileCoords = (color == PieceColor::Black) ? sf::Vector2u(4, 0) : sf::Vector2u(4, 1);
        break;
    case PieceType::King:
        m_TileCoords = (color == PieceColor::Black) ? sf::Vector2u(5, 0) : sf::Vector2u(5, 1);
        break;
    }

    sf::Vector2i position = {static_cast<int32_t>(m_TileCoords.x * PIECE_SIZE), static_cast<int32_t>(m_TileCoords.y * PIECE_SIZE)};
    sf::Vector2i size = {static_cast<int32_t>(PIECE_SIZE), static_cast<int32_t>(PIECE_SIZE)};

    m_Sprite.setTextureRect(sf::IntRect(position, size));
    m_Sprite.setScale({SPRITE_SCALING, SPRITE_SCALING});
}

void AddRookMoves(sf::Vector2f source, std::vector<Move> &moves)
{
    for (uint8_t column = 0; column != ROW_COUNT; ++column)
    {
        if (column != source.x)
        {
            moves.push_back({{static_cast<uint8_t>(source.x), static_cast<uint8_t>(source.y)},
                             {static_cast<uint8_t>(column), static_cast<uint8_t>(source.y)}});
        }
    }
    for (uint8_t row = 0; row != ROW_COUNT; ++row)
    {
        if (row != source.y)
        {
            moves.push_back({{static_cast<uint8_t>(source.x), static_cast<uint8_t>(source.y)},
                             {static_cast<uint8_t>(source.x), static_cast<uint8_t>(row)}});
        }
    }
}

void AddBishopMoves(sf::Vector2f source, std::vector<Move> &moves)
{
    // Complicated maths that I don't remember anymore.
    // What we do here is basically figure out how far away
    // we are from each of the corners, and use it as the
    // start and end of the diagonal line.
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
        moves.push_back({{static_cast<uint8_t>(source.x), static_cast<uint8_t>(source.y)},
                         {static_cast<uint8_t>(source.x + i), static_cast<uint8_t>(source.y + i)}});
    }

    uint8_t secondary_axis_start = std::min(dist_x_start, dist_y_end);
    uint8_t secondary_axis_end = std::min(dist_x_end, dist_y_start);

    for (int i = -secondary_axis_start; i <= secondary_axis_end; ++i)
    {
        if (i == 0)
            continue;
        moves.push_back({{static_cast<uint8_t>(source.x), static_cast<uint8_t>(source.y)},
                         {static_cast<uint8_t>(source.x + i), static_cast<uint8_t>(source.y - i)}});
    }
}

std::vector<Move> ChessPiece::GetAvailableMoves()
{
    sf::Vector2f source = m_Sprite.getPosition() / (float)CELL_SIZE;
    std::vector<Move> moves;
    switch (m_Type)
    {
    case PieceType::Pawn:
    {
        int dir = (m_Color == PieceColor::White ? -1 : 1);

        Move one_step = {{static_cast<uint8_t>(source.x), static_cast<uint8_t>(source.y)},
                         {static_cast<uint8_t>(source.x), static_cast<uint8_t>(source.y + dir)}};
        if (one_step.destination.x < ROW_COUNT && one_step.destination.y < ROW_COUNT)
            moves.push_back(one_step);

        Move two_step = {{static_cast<uint8_t>(source.x), static_cast<uint8_t>(source.y)},
                         {static_cast<uint8_t>(source.x), static_cast<uint8_t>(source.y + dir * 2)}};
        if (two_step.destination.x < ROW_COUNT && two_step.destination.y < ROW_COUNT)
            moves.push_back(two_step);

        Move take_right = {{static_cast<uint8_t>(source.x), static_cast<uint8_t>(source.y)},
                           {static_cast<uint8_t>(source.x + 1), static_cast<uint8_t>(source.y + dir)}};
        if (take_right.destination.x < ROW_COUNT && take_right.destination.y < ROW_COUNT)
            moves.push_back(take_right);

        Move take_left = {{static_cast<uint8_t>(source.x), static_cast<uint8_t>(source.y)},
                          {static_cast<uint8_t>(source.x - 1), static_cast<uint8_t>(source.y + dir)}};
        if (take_left.destination.x < ROW_COUNT && take_left.destination.y < ROW_COUNT)
            moves.push_back(take_left);
        break;
    }
    case PieceType::Rook:
        AddRookMoves(source, moves);
        break;
    case PieceType::Bishop:
        AddBishopMoves(source, moves);
        break;
    case PieceType::Knight:
    {
        for (int i = -2; i <= 2; ++i)
        {
            for (int j = -2; j <= 2; ++j)
            {
                if (abs(i) == abs(j) || i == 0 || j == 0)
                    continue;
                if ((source.x + i) < ROW_COUNT && (source.x + i) >= 0 && (source.y + j) < ROW_COUNT && (source.y + j) >= 0)
                    moves.push_back({{static_cast<uint8_t>(source.x), static_cast<uint8_t>(source.y)},
                                     {static_cast<uint8_t>(source.x + i), static_cast<uint8_t>(source.y + j)}});
            }
        }
        break;
    }
    case PieceType::Queen:
    {
        AddRookMoves(source, moves);
        AddBishopMoves(source, moves);
        break;
    }
    case PieceType::King:
    {
        for (int i = -1; i <= 1; ++i)
        {
            for (int j = -1; j <= 1; ++j)
            {
                if (i == 0 && j == 0)
                    continue;
                if ((source.x + i) < ROW_COUNT && (source.x + i) >= 0 && (source.y + j) < ROW_COUNT && (source.y + j) >= 0)
                    moves.push_back({{static_cast<uint8_t>(source.x), static_cast<uint8_t>(source.y)},
                                     {static_cast<uint8_t>(source.x + i), static_cast<uint8_t>(source.y + j)}});
            }
        }
        break;
    }
    }
    return moves;
}
