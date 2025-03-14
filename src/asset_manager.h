#pragma once

const std::string BOARD_TEXTURE_PATH = "../../../assets/board.png";
const std::string PIECE_TEXTURE_PATH = "../../../assets/pieces.png";
const std::string FONT_PATH = "../../../assets/isocpeur.ttf";

class AssetManager
{
public:
    AssetManager()
        : m_BoardTexture(BOARD_TEXTURE_PATH),
          m_PieceTexture(PIECE_TEXTURE_PATH),
          m_Font(FONT_PATH) {};

    sf::Texture &GetBoardTexture()
    {
        return m_BoardTexture;
    }

    sf::Texture &GetPieceTexture()
    {
        return m_PieceTexture;
    }

    sf::Font &GetFont()
    {
        return m_Font;
    }

private:
    sf::Texture m_BoardTexture;
    sf::Texture m_PieceTexture;
    sf::Font m_Font;
};