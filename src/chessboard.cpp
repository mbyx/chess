#include <SFML/Graphics.hpp>
#include <iostream>
#include "chessboard.h"

ChessBoard::ChessBoard(sf::Texture &boardTexture, sf::Texture &pieceTexture) : m_Sprite(boardTexture)
{
    m_Sprite.setScale({SPRITE_SCALING, SPRITE_SCALING});

    m_Pieces[0][0] = ChessPiece(ChessPiece::PieceType::Rook, ChessPiece::PieceColor::Black, pieceTexture);
    m_Pieces[0][1] = ChessPiece(ChessPiece::PieceType::Knight, ChessPiece::PieceColor::Black, pieceTexture);
    m_Pieces[0][2] = ChessPiece(ChessPiece::PieceType::Bishop, ChessPiece::PieceColor::Black, pieceTexture);
    m_Pieces[0][3] = ChessPiece(ChessPiece::PieceType::Queen, ChessPiece::PieceColor::Black, pieceTexture);
    m_Pieces[0][4] = ChessPiece(ChessPiece::PieceType::King, ChessPiece::PieceColor::Black, pieceTexture);
    m_Pieces[0][5] = ChessPiece(ChessPiece::PieceType::Bishop, ChessPiece::PieceColor::Black, pieceTexture);
    m_Pieces[0][6] = ChessPiece(ChessPiece::PieceType::Knight, ChessPiece::PieceColor::Black, pieceTexture);
    m_Pieces[0][7] = ChessPiece(ChessPiece::PieceType::Rook, ChessPiece::PieceColor::Black, pieceTexture);
    for (size_t column = 0; column != ROW_COUNT; ++column)
    {
        m_Pieces[1][column] = ChessPiece(ChessPiece::PieceType::Pawn, ChessPiece::PieceColor::Black, pieceTexture);
    }

    for (size_t column = 0; column != ROW_COUNT; ++column)
    {
        m_Pieces[6][column] = ChessPiece(ChessPiece::PieceType::Pawn, ChessPiece::PieceColor::White, pieceTexture);
    }
    m_Pieces[7][0] = ChessPiece(ChessPiece::PieceType::Rook, ChessPiece::PieceColor::White, pieceTexture);
    m_Pieces[7][1] = ChessPiece(ChessPiece::PieceType::Knight, ChessPiece::PieceColor::White, pieceTexture);
    m_Pieces[7][2] = ChessPiece(ChessPiece::PieceType::Bishop, ChessPiece::PieceColor::White, pieceTexture);
    m_Pieces[7][3] = ChessPiece(ChessPiece::PieceType::Queen, ChessPiece::PieceColor::White, pieceTexture);
    m_Pieces[7][4] = ChessPiece(ChessPiece::PieceType::King, ChessPiece::PieceColor::White, pieceTexture);
    m_Pieces[7][5] = ChessPiece(ChessPiece::PieceType::Bishop, ChessPiece::PieceColor::White, pieceTexture);
    m_Pieces[7][6] = ChessPiece(ChessPiece::PieceType::Knight, ChessPiece::PieceColor::White, pieceTexture);
    m_Pieces[7][7] = ChessPiece(ChessPiece::PieceType::Rook, ChessPiece::PieceColor::White, pieceTexture);

    for (size_t row = 0; row != ROW_COUNT; ++row)
    {
        for (size_t column = 0; column != ROW_COUNT; ++column)
        {
            auto &piece = m_Pieces[row][column];
            if (piece.has_value())
            {
                piece->GetSprite().setPosition({(float)column * (float)PIECE_SIZE * SPRITE_SCALING, (float)row * (float)PIECE_SIZE * SPRITE_SCALING});
            }
        }
    }
}

void ChessBoard::Draw(sf::RenderWindow &window)
{
    window.draw(m_Sprite);
    for (size_t row = 0; row != ROW_COUNT; ++row)
    {
        for (size_t column = 0; column != ROW_COUNT; ++column)
        {
            std::optional<ChessPiece> piece = m_Pieces[row][column];
            if (piece.has_value())
            {
                piece.value().Draw(window);
            }
        }
    }
}