#include "Tiled.h"
#include "RessourcesRepo.h"
#include "LMap.h"

namespace {
    const char* BACKGROUND = "Background";
    const char* WALLS = "Walls";
    const char* WARPS = "Warps";
    const char* WARP = "warp";
    const char* LOAD = "load";
    const char* WALL_TILE = "wall";
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
                    const TileMap& tilemap)
    {
        bool success = true;

        m_pWindow = &p_window;

        m_name = tilemap.GetName();
        m_heigth = tilemap.GetHeigth();
        m_width = tilemap.GetWidth();
        m_tileheight = tilemap.GetTileheight();
        m_tilewidth = tilemap.GetTilewidth();

        // Find background and walls tileset among tileset read
        const TileMap::TilesetNode* backgroundTileset = nullptr;
        const TileMap::LayerNode* backgroundNode = nullptr;
        const TileMap::ObjectGroup* warps = nullptr;

        if (success) {
            success = tilemap.FindLayerNode(BACKGROUND, &backgroundNode, &backgroundTileset);
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
                                m_tiles[i][j].m_blocked = backgroundTileset->m_tileSet.getType(index) == WALL_TILE;
                            }
                        }
                }

            // This is a copy because m_properties would need to be non const... whatever
            for (TileMap::Object obj : warps->m_objects)
                {
                    Zone warp;

                    warp.m_x = obj.m_x;
                    warp.m_y = obj.m_y;
                    warp.m_h = obj.m_h;
                    warp.m_w = obj.m_w;
                    warp.m_name = obj.m_name;
                    if (obj.m_type == LOAD) {
                        m_loads[warp.m_name] = warp;
                    } else if (obj.m_type == WARP) {
                        m_warps[warp.m_name] = warp;
                    }


                }
        }

        return success;
    }

    void LMap::Render() const {
        int x = m_x;
        int y = m_y;

        int nb_h = m_pWindow->getHeight() / m_tileheight + 1;
        int nb_w = m_pWindow->getWidth() / m_tilewidth + 1;

        int leftmost = x > 0 ? 0 : -x/m_tilewidth;
        int rightmost = leftmost + nb_w < m_width ? leftmost + nb_w : m_width;

        int upmost = y > 0 ? 0 : -y/m_tileheight;
        int downmost = upmost + nb_h + 1 < m_heigth ? upmost + nb_h + 1 : m_heigth;

        for (int i = upmost; i < downmost; i++)
            {
                for (int j = leftmost; j < rightmost; j++)
                    {
                        int tilePosX = j*m_tilewidth + x;
                        int tilePosY = i*m_tileheight + y;
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
        m_x = m_pWindow->getWidth()/2 - middle_x;
        m_y = m_pWindow->getHeight()/2 - middle_y;
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
        // Index of tile
        int ix = x/m_tilewidth;
        int iy = y/m_tileheight;

        return !(ix >= 0 && ix < m_width && iy >= 0 && iy < m_heigth) ||
            m_tiles[iy][ix].m_blocked;
    }

    /********************************************************************
      Check if the rectangle is in the warp. Set the p_rWarp parameter
      if it is.
     *******************************************************************/
    bool LMap::inWarp(int x, int y, int w, int h, std::string& p_rWarp) const
    {
        bool inWarp = false;
        std::map<std::string, Zone>::const_iterator it, itend = m_warps.end();
        for (it = m_warps.begin(); it != itend && !inWarp; ++it) {
            const Zone& warp = it->second;
            inWarp = ((x+w) >= warp.m_x && x < (warp.m_x + warp.m_w)
                      && (y+h) >= warp.m_y && y < (warp.m_y + warp.m_h));
            p_rWarp = warp.m_name;
        }

        return inWarp;
    }
}

