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
	const int ALIGN_THRESHOLD = 3;
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

    bool LMap::Init(const RessourcesRepo& p_ressourceRepo,
                    const LWindow& p_window,
                    const std::string& p_name,
                    const tiled::TileMap& tilemap)
    {
        bool success = true;

        m_pWindow = &p_window;

        m_name = p_name;
        m_heigth = tilemap.m_heigth;
        m_width = tilemap.m_width;
        m_tileheight = tilemap.m_tileheight;
        m_tilewidth = tilemap.m_tilewidth;

        // Find background and walls tileset
        const std::map<std::string, tiled::Layer>::const_iterator itrBackgroundLayer = tilemap.m_layers.find(BACKGROUND);
        const std::map<std::string, tiled::ObjectGroup>::const_iterator itrWarps = tilemap.m_objects.find(WARPS);

        if (success) {
            success = itrBackgroundLayer != tilemap.m_layers.end();
            success &= itrWarps != tilemap.m_objects.end();
        }

        // Initialize tiles of map
        if (success) {
            const tiled::Layer& backgroundLayer = itrBackgroundLayer->second;

            m_tiles = new Tile*[m_heigth];
            for (int i=0; i<m_heigth; i++) {
                m_tiles[i] = new Tile[m_width];
                for (int j=0; j<m_width; j++) {

                    int index = backgroundLayer.m_data[i][j];
                    if (index > 0) {

                        const tiled::TilesetRef& tilesetRef = tilemap.FindTilesetRef(index);
                        index -= tilesetRef.m_firstGid;

                        const tiled::TileSet& tileSet = p_ressourceRepo.getTileSet(RessourcesRepo::getRessourceName(tilesetRef.m_source));

                        const LTexture& texture = p_ressourceRepo.getImage(RessourcesRepo::getRessourceName(tileSet.m_image));
                        int nbTilesInCol = texture.getWidth() / m_tilewidth;
                        int x = (index % nbTilesInCol)*m_tilewidth;
                        int y = (index / nbTilesInCol)*m_tileheight;

                        m_tiles[i][j].m_rect = {x, y, m_tilewidth, m_tileheight};
                        std::string tileType = "";
                        std::map<int, tiled::Tile>::const_iterator itrTile = tileSet.m_tiles.find(index);
                        if (itrTile != tileSet.m_tiles.end()) {
                            const tiled::Tile& tile = itrTile->second;
                            tileType = tile.m_type;
                        }

                        m_tiles[i][j].m_blocked = tileType == WALL_TILE;
                        m_tiles[i][j].m_texture = &texture;
                    }
                }
            }

            // Initialize the warps in the map
            for (const tiled::Object& obj : itrWarps->second.m_objects) {
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

        for (int i = upmost; i < downmost; i++) {
            for (int j = leftmost; j < rightmost; j++) {
                if (m_tiles[i][j].m_texture != nullptr) {
                    int tilePosX = j*m_tilewidth + x;
                    int tilePosY = i*m_tileheight + y;
                    m_tiles[i][j].m_texture->render(tilePosX, tilePosY, &m_tiles[i][j].m_rect);
                }
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

        for (; ix <= endx && !blocked; ix++) {
            for (int j = iy; j <= endy && !blocked; j++) {
                blocked = m_tiles[j][ix].m_blocked;
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

    /*********************************************************************
      Return the number of pixel a rectangle can move to not be blocked
      horizontally. Up to 3 pixels in absolute value.
    **********************************************************************/
    int LMap::AlignH(const Rectangle& p_Rec) const {
    
        int nbPixelLeft = m_tilewidth - p_Rec.m_x % m_tilewidth + 1;
        int nbPixelRight = (p_Rec.m_x + p_Rec.m_w) % m_tilewidth;

        if (nbPixelLeft <= ALIGN_THRESHOLD && 
            !isBlocked(Rectangle(p_Rec.m_x + nbPixelLeft, p_Rec.m_y, p_Rec.m_w, p_Rec.m_h)))
            {
                return nbPixelLeft;
            }

        if (nbPixelRight <= ALIGN_THRESHOLD &&
            !isBlocked(Rectangle(p_Rec.m_x - nbPixelRight, p_Rec.m_y, p_Rec.m_w, p_Rec.m_h)))
            {
                return 0-nbPixelRight;
            }

        return 0;
    }
}

