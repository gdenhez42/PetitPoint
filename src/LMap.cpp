#include "Tiled.h"
#include "RessourcesRepo.h"
#include "LMap.h"

namespace {
    const char* BACKGROUND = "Background";
    const char* WALLS = "Walls";
}

namespace pp
{

LMap::LMap()
: m_tiles(nullptr)
{
    //ctor
}

LMap::~LMap()
{
    //dtor
    if (m_tiles == nullptr) return;
    for (int i=0; i<m_heigth; i++)
    {
        delete[] m_tiles[i];
    }
    delete[] m_tiles;
    m_tiles = nullptr;
}

bool LMap::Init(const LWindow& p_window, const RessourcesRepo& p_ressourceRepo, const std::string& p_filename)
{
    bool success = true;

    m_pWindow = &p_window;

    const TileMap& tilemap = p_ressourceRepo.getMap(p_filename);

    m_heigth = tilemap.GetHeigth();
    m_width = tilemap.GetWidth();
    m_tileheight = tilemap.GetTileheight();
    m_tilewidth = tilemap.GetTilewidth();

    // Find background and walls tileset among tileset read
    const TileMap::TilesetNode* backgroundTileset = nullptr;
    const TileMap::TilesetNode* wallsTileset = nullptr;
    const TileMap::LayerNode* backgroundNode = nullptr;
    const TileMap::LayerNode* wallsNode = nullptr;


    if (success) {
        success = tilemap.FindLayerNode(BACKGROUND, &backgroundNode, &backgroundTileset);
        success &= tilemap.FindLayerNode(WALLS, &wallsNode, &wallsTileset);
    }

    // Initialize tiles of map
    if (success) {
        m_background = backgroundTileset->GetTexture();
        int nbTilesInCol = m_background->getWidth() / m_tilewidth;

        if (m_heigth > 0)
        {
            m_tiles = new Tile*[m_heigth];
            for (int i=0; i<m_heigth; i++)
            {
                m_tiles[i] = new Tile[m_width];
                for (int j=0; j<m_width; j++) {
                    int index = backgroundNode->m_tiles[i][j] - backgroundTileset->m_firstGid;
                    int x = (index % nbTilesInCol)*m_tilewidth;
                    int y = (index / nbTilesInCol)*m_tileheight;

                    m_tiles[i][j].m_rect = {x, y, m_tilewidth, m_tileheight};
                    m_tiles[i][j].m_blocked = (wallsNode->m_tiles[i][j] - wallsTileset->m_firstGid == 0);
                }
            }
        }
    }

    return success;
}

void LMap::Render() const {
    int x = m_x;
    int y = m_y;

    int wh = m_pWindow->getHeight();
    int ww = m_pWindow->getWidth();

    int leftmost = x > 0 ? x/m_tilewidth : 0;
    int rightmost = (x + ww)/m_tilewidth + 1 < m_width ? (x + ww)/m_tilewidth + 1 : m_width;

    int upmost = y > 0 ? y/m_tileheight : 0;
    int downmost = (y + wh)/m_tileheight + 1 < m_heigth ? (y + wh)/m_tileheight + 1 : m_heigth;

    for (int i = upmost; i < downmost; i++)
    {
        for (int j = leftmost; j < rightmost; j++)
        {
            int tilePosX = j*m_tilewidth - x;
            int tilePosY = i*m_tileheight - y;
            m_background->render(tilePosX, tilePosY, &m_tiles[i][j].m_rect);
        }
    }
}

void LMap::Update (int x, int y)
{
    m_x = x;
    m_y = y;
}

bool LMap::isBlocked(int x, int y) const
{
    // Effective position in the room
    int ePosX = x + m_x;
    int ePosY = y + m_y;

    // Index of tile
    int ix = ePosX/m_tilewidth;
    int iy = ePosY/m_tileheight;

    return !(ix >= 0 && ix < m_width && iy >= 0 && iy < m_heigth) ||
           m_tiles[iy][ix].m_blocked;
}

}
