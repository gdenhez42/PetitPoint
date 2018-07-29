#ifndef TILESET_H
#define TILESET_H

#include "LTexture.h"
#include <string>
#include <vector>
#include <map>
#include <SDL2/SDL.h>

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
        std::string m_name;
        int m_width;
        int m_heigth;
        LayerNode(const std::string& name, int width, int heigth, const std::string& content);
        LayerNode(const LayerNode&);
        LayerNode(LayerNode&&);
        ~LayerNode();
        LayerNode& operator=(LayerNode);
    };

    struct Tile {
        SDL_Rect m_rect;
        bool m_blocked;
    };

    public:
        TileMap();
        TileMap(const TileMap&);
        TileMap(TileMap&&);
        virtual ~TileMap();
        TileMap& operator=(TileMap);

        int getX() const {return m_x;}
        int getY() const {return m_y;}

        bool Init(const LWindow& p_window, const RessourcesRepo& p_ressourceRepo, const std::string& p_filename);
        void Render();
        void Update(int x, int y);
        bool IsBlocked(int x, int y);

    private:
        const TilesetNode& FindTileset(int gid) const;

        std::vector<TilesetNode> m_tilesets;
        std::vector<LayerNode> m_layers;

        int m_width;
        int m_heigth;
        int m_tilewidth;
        int m_tileheight;

        // To render the tiled map
        const LWindow* m_pWindow;
        const LTexture* m_background;
        Tile** m_tiles;
        int m_x;
        int m_y;
};

}

#endif // TILESET_H
