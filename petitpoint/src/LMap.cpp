#include "Tiled.h"
#include "RessourcesRepo.h"
#include "LMap.h"
#include <cstdlib>

namespace {
    const char* BACKGROUND = "Background";
    const char* WALLS = "Walls";
    const char* WARPS = "Warps";
    const char* WARP = "warp";
    const char* LOAD = "load";
    const char* WALL_TILE = "wall";
    const std::string X = "X";
    const std::string Y = "Y";
    const std::string H = "H";
    const std::string W = "W";
    const std::string DEST = "dest";
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

        // Find background layer
        const std::map<std::string, tiled::Layer>::const_iterator itrBackgroundLayer = tilemap.m_layers.find(BACKGROUND);

        if (success) {
            success = itrBackgroundLayer != tilemap.m_layers.end();
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
        }

        // Initialize warps
        if (success) {
            const std::map<std::string, tiled::ObjectGroup>::const_iterator itrWarps = tilemap.m_objects.find(WARPS);
            if (itrWarps != tilemap.m_objects.end()) {

                for (const tiled::Object& object : itrWarps->second.m_objects) {
                    std::map<std::string,std::string>::const_iterator itrLoadX = object.m_properties.find(X);
                    std::map<std::string,std::string>::const_iterator itrLoadY = object.m_properties.find(Y);
                    std::map<std::string,std::string>::const_iterator itrLoadW = object.m_properties.find(W);
                    std::map<std::string,std::string>::const_iterator itrLoadH = object.m_properties.find(H);
                    std::map<std::string,std::string>::const_iterator itrLoadDest = object.m_properties.find(DEST);
                    std::map<std::string,std::string>::const_iterator itrPropsEnd = object.m_properties.end();

                    if (itrLoadX != itrPropsEnd &&
                        itrLoadY != itrPropsEnd &&
                        itrLoadW != itrPropsEnd &&
                        itrLoadH != itrPropsEnd &&
                        itrLoadDest != itrPropsEnd) {

                        LoadZone load;

                        load.m_rect = Rectangle(atoi(object.m_properties.at(X).c_str()),
                                                atoi(object.m_properties.at(Y).c_str()),
                                                atoi(object.m_properties.at(H).c_str()),
                                                atoi(object.m_properties.at(W).c_str()));
                        load.m_room = object.m_properties.at(DEST);
                        WarpZone warp;
                        warp.m_rect = Rectangle(object.m_x, object.m_y, object.m_h, object.m_w);
                        warp.m_load = load;
                        m_warps.push_back(warp);
                    }
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
    bool LMap::getWarp(const Rectangle& p_Rec, WarpZone& p_rWarp) const
    {
        bool inWarp = false;
        std::vector<WarpZone>::const_iterator it, itend = m_warps.end();
        for (it = m_warps.begin(); it != itend && !inWarp; ++it) {
            inWarp = AreColliding(p_Rec, it->m_rect);
            p_rWarp = *it;
        }

        return inWarp;
    }

}

