/****************************************************************
Utility classes to read tiled map (see https://www.mapeditor.org/)
****************************************************************/

#ifndef TILED_H
#define TILED_H

#include "LTexture.h"
#include "LAnimation.h"
#include <string>
#include <vector>
#include <map>

class RessourcesRepo;

namespace pp::tiled
{
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

    struct Frame {
        int m_tileId;
    };

    struct Animation {
        std::vector<Frame> m_frames;
    };

    struct Tile {
        int m_id;
        std::string m_type;
        ObjectGroup m_objects;
        Animation m_animation;
    };

    struct TilesetRef {
        int m_firstGid;
        std::string m_source;
    };

    struct Layer {
        std::string m_name;
        int m_width;
        int m_height;
        std::vector<std::vector<int> > m_data;
    };

    struct TileSet
    {
        bool Init(const std::string& p_filename);

        std::string m_name;
        int m_width;
        int m_height;
        int m_count;
        int m_columns;
        std::string m_image;
        std::map<int, Tile> m_tiles;
    };

    struct TileMap
    {
        bool Init(const std::string& p_filename);
        const TilesetRef& FindTilesetRef(int p_Gid) const;

        std::vector<TilesetRef> m_tilesets;
        std::map<std::string, Layer> m_layers;
        std::map<std::string, ObjectGroup> m_objects;

        int m_width;
        int m_heigth;
        int m_tilewidth;
        int m_tileheight;
    };
}

#endif // TILED_H
