
// TODO: Think of a better way to access paths.
// When debugging in VSCode, I want these paths, but when executing, I just want to
// check the current directory for an assets folder.
const std::string BOARD_TEXTURE_PATH = "../../../assets/board.png";
const std::string PIECE_TEXTURE_PATH = "../../../assets/pieces.png";

class AssetManager
{
public:
    // TODO: Maybe change this to a singleton.
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