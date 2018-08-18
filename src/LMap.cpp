#include "Tiled.h"
#include "RessourcesRepo.h"
#include "LMap.h"

namespace {
    const char* BACKGROUND = "Background";
    const char* WALLS = "Walls";
    const char* WARPS = "Warps";
    const char* WARP = "warp";
    const char* LOAD = "load";
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

bool LMap::Init(const LWindow& p_window,
                const RessourcesRepo& p_ressourceRepo,
                const std::string& p_filename,
                const std::string& p_name)
{
    bool success = true;

    m_pWindow = &p_window;

    const TileMap& tilemap = p_ressourceRepo.getMap(p_filename);

    m_name = p_name;
    m_heigth = tilemap.GetHeigth();
    m_width = tilemap.GetWidth();
    m_tileheight = tilemap.GetTileheight();
    m_tilewidth = tilemap.GetTilewidth();

    // Find background and walls tileset among tileset read
    const TileMap::TilesetNode* backgroundTileset = nullptr;
    const TileMap::TilesetNode* wallsTileset = nullptr;
    const TileMap::LayerNode* backgroundNode = nullptr;
    const TileMap::LayerNode* wallsNode = nullptr;
    const TileMap::ObjectGroup* warps = nullptr;


    if (success) {
        success = tilemap.FindLayerNode(BACKGROUND, &backgroundNode, &backgroundTileset);
        success &= tilemap.FindLayerNode(WALLS, &wallsNode, &wallsTileset);
        success &= tilemap.FindObjectGroup(WARPS, &warps);
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

        // This is a copy because m_properties would need to be non const... wathever
        for (TileMap::Object obj : warps->m_objects)
        {
            Zone warp;

            warp.m_x = obj.m_x;
            warp.m_y = obj.m_y;
            warp.m_h = obj.m_h;
            warp.m_w = obj.m_w;
            if (obj.m_type == LOAD) {
                warp.m_name = obj.m_properties[LOAD];
                m_loads[warp.m_name] = warp;
            } else if (obj.m_type == WARP) {
                warp.m_name = obj.m_properties[WARP];
                m_warps[warp.m_name] = warp;

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

void LMap::Update(const std::string& p_load)
{
    const Zone& load = m_loads[p_load];
    int middle_x = load.m_x + load.m_w/2;
    int middle_y = load.m_y + load.m_h/2;
    m_x = middle_x - m_pWindow->getWidth()/2;
    m_y = middle_y - m_pWindow->getHeight()/2;
}

std::vector<std::string> LMap::getLoads() const
{
    std::vector<std::string> warps;
    std::map<std::string, Zone>::const_iterator it, itend = m_loads.end();
    for(it = m_loads.begin(); it != itend; ++it) {
        warps.push_back(it->first);
    }
    return warps;
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

bool LMap::isWarp(int x, int y, std::string& p_rWarp) const
{
    // Effective position in the room
    int ePosX = x + m_x;
    int ePosY = y + m_y;

    bool isWarp = false;
    std::map<std::string, Zone>::const_iterator it, itend = m_warps.end();
    for (it = m_warps.begin(); it != itend && !isWarp; ++it) {
        const Zone& warp = it->second;
        isWarp = (ePosX >= warp.m_x && ePosX < (warp.m_x + warp.m_w)
                  && ePosY >= warp.m_y && ePosY < (warp.m_y + warp.m_h));
        p_rWarp = warp.m_name;
    }

    return isWarp;
}

bool LMap::inWarp(int x, int y, int w, int h) const
{
    // Effective position in the room
    int ePosX = x + m_x;
    int ePosY = y + m_y;

    bool inWarp = false;
    std::map<std::string, Zone>::const_iterator it, itend = m_warps.end();
    for (it = m_warps.begin(); it != itend && !inWarp; ++it) {
        const Zone& warp = it->second;
        inWarp = ((ePosX+w) >= warp.m_x && ePosX < (warp.m_x + warp.m_w)
                  && (ePosY+h) >= warp.m_y && ePosY < (warp.m_y + warp.m_h));
    }

    return inWarp;

}

}
