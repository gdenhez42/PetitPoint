/****************************************************************
Utility classes to read tiled map (see https://www.mapeditor.org/)

****************************************************************/

#ifndef TILESET_H
#define TILESET_H

#include "LTexture.h"
#include "LAnimation.h"
#include <string>
#include <vector>
#include <map>
#include <SDL2/SDL.h>

class RessourcesRepo;

namespace pp
{

class TileSet
{
    public:
        TileSet();
        virtual ~TileSet();

        bool Init(const RessourcesRepo& p_ressourceRepo, const std::string& p_filename);
        const LAnimation& getAnimation(int id) const;
        const std::string& getName() const { return m_name; }
        const std::string& getType(int id) const;

    private:
        std::string m_name;
        int m_width;
        int m_height;
        int m_count;
        int m_columns;
        const LTexture* m_image;

        std::map<int, LAnimation> m_animations;
        std::map<int, std::string> m_types;

        friend class TileMap;
};

class TileMap
{
    public:

        // Tiled structures
        struct TilesetNode {
            int m_firstGid;
            const TileSet& m_tileSet;
            TilesetNode(int p_firstGid, const TileSet& p_tileSet);
            const LTexture* GetTexture() const;
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
        struct Object {
            int m_id;
            int m_gid;
            int m_x;
            int m_y;
            int m_w;
            int m_h;
            std::string m_type;
            std::string m_name;
            std::map<std::string,std::string> m_properties;
        };

        struct ObjectGroup {
            std::string m_name;
            std::vector<Object> m_objects;
        };

        TileMap();
        TileMap(const TileMap&);
        TileMap(TileMap&&);
        virtual ~TileMap();
        TileMap& operator=(TileMap);

        int GetWidth() const { return m_width; }
        int GetHeigth() const { return m_heigth; }
        int GetTilewidth() const { return m_tilewidth; }
        int GetTileheight() const { return m_tileheight; }
        const std::string& GetName() const { return m_name; }

        bool Init(const RessourcesRepo& p_ressourceRepo, const std::string& p_name);
        bool FindLayerNode(const std::string& p_name, const LayerNode** p_ppLayer) const;
        bool FindLayerNode(const std::string&, const LayerNode**, const TilesetNode**) const;
        bool FindObjectGroup(const std::string&, const ObjectGroup**) const;
        const TilesetNode& FindTileset(int gid) const;

    private:

        std::vector<TilesetNode> m_tilesets;
        std::vector<LayerNode> m_layers;
        std::vector<ObjectGroup> m_objects;

        int m_width;
        int m_heigth;
        int m_tilewidth;
        int m_tileheight;
        std::string m_name;
};

}

#endif // TILESET_H
