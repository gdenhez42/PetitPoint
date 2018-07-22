#ifndef TILESET_H
#define TILESET_H

#include "LTexture.h"
#include <string>
#include <vector>
#include <map>

class LWindow;
class RessourcesRepo;

namespace pp
{
class TileSet
{
    public:
        TileSet();
        virtual ~TileSet();

        bool Init(const LWindow& window, const std::string& p_filename);

    private:
        std::string m_name;
        int m_width;
        int m_height;
        int m_count;
        int m_columns;
        LTexture m_image;

        friend class TileMap;
};

class TileMap
{
    struct TilesetNode {
        int m_firstGid;
        const TileSet& m_tileSet;
        TilesetNode(int p_firstGid, const TileSet& p_tileSet);
    };

    struct LayerNode {
        int** m_tiles;
        int m_width;
        int m_heigth;
        LayerNode(int width, int heigth, const std::string& content);
        LayerNode(const LayerNode&);
        LayerNode(LayerNode&&);
        ~LayerNode();
        LayerNode& operator=(LayerNode);
    };

    public:
        TileMap();
        virtual ~TileMap();

        bool Init(const RessourcesRepo& p_ressourceRepo, const std::string& p_filename);
    private:
        std::vector<TilesetNode> m_tilesets;
        std::map<std::string,LayerNode> m_layers;

        int m_width;
        int m_heigth;
        int m_tilewidth;
        int m_tileheight;
};

}

#endif // TILESET_H
