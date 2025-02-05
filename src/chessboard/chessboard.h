
#include <SFML/Graphics.hpp>

#include "../chesspiece/chesspiece.h"

class ChessBoard
{
public:
    ChessBoard(sf::Texture &texture, sf::Texture &pieceTexture);

    void ConstructFromFEN(std::string fen, sf::Texture &pieceTexture);
    void Draw(sf::RenderWindow &window);

    std::vector<ChessPiece> GetPiecesByType(ChessPiece::PieceType type);
    std::vector<ChessPiece> GetPiecesByColor(ChessPiece::PieceColor color);
    std::optional<ChessPiece> &GetPieceAt(sf::Vector2u position);

    void PerformMove(Move move);
    void UnPerformMove(Move move);

private:
    std::optional<ChessPiece> m_Pieces[ROW_COUNT][ROW_COUNT];
    std::optional<ChessPiece> m_LastTakenPiece;
    sf::Sprite m_Sprite;
};