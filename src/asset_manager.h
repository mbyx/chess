
const std::string BOARD_TEXTURE_PATH = "../../../assets/board.png";
const std::string PIECE_TEXTURE_PATH = "../../../assets/pieces.png";

class AssetManager
{
public:
    AssetManager() : m_BoardTexture(BOARD_TEXTURE_PATH), m_PieceTexture(PIECE_TEXTURE_PATH) {};

    sf::Texture &GetBoardTexture()
    {
        return m_BoardTexture;
    }

    sf::Texture &GetPieceTexture()
    {
        return m_PieceTexture;
    }

private:
    sf::Texture m_BoardTexture;
    sf::Texture m_PieceTexture;
};