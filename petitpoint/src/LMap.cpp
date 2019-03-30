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
                Zone warp(obj.m_x, obj.m_y, obj.m_w, obj.m_h, obj.m_name);
                if (obj.m_type == LOAD) {
                    m_loads.insert(std::make_pair(warp.m_name, warp));
                } else if (obj.m_type == WARP) {
                    m_warps.insert(std::make_pair(warp.m_name, warp));
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
        const Zone& load = m_loads.find(p_load)->second;
        int middle_x = load.m_rec.m_x + load.m_rec.m_w/2;
        int middle_y = load.m_rec.m_y + load.m_rec.m_h/2;
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

    bool LMap::isBlocked(const Rectangle& p_Rec) const
    {
		bool blocked = (p_Rec.m_x < 0 || p_Rec.m_x + p_Rec.m_w >= m_width*m_tilewidth ||
			p_Rec.m_y < 0 || p_Rec.m_y + p_Rec.m_h >= m_heigth*m_tileheight);

		int ix = p_Rec.m_x / m_tilewidth;
		int iy = p_Rec.m_y / m_tileheight;

		int endx = (p_Rec.m_x + p_Rec.m_w - 1) / m_tilewidth;
		int endy = (p_Rec.m_y + p_Rec.m_h - 1) / m_tileheight;

		for (; ix <= endx; ix++) {
			for (; iy <= endy; iy++) {
				blocked = m_tiles[iy][ix].m_blocked;
			}
		}

        return blocked;
    }

    /********************************************************************
      Check if the rectangle is in the warp. Set the p_rWarp parameter
      if it is.
     *******************************************************************/
    bool LMap::inWarp(const Rectangle& p_Rec, std::string& p_rWarp) const
    {
        bool inWarp = false;
        std::map<std::string, Zone>::const_iterator it, itend = m_warps.end();
        for (it = m_warps.begin(); it != itend && !inWarp; ++it) {
            const Zone& warp = it->second;
            inWarp = AreColliding(p_Rec, warp.m_rec);
            p_rWarp = warp.m_name;
        }

        return inWarp;
    }
}

