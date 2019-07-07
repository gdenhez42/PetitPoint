#include "Tiled.h"
#include "tinyxml2.h"
#include "RessourcesRepo.h"

namespace {

    const char* NUMBERS = "0123456789";
    const char* TILEID = "tileid";
    const char* TYPE = "type";
    const char* SOURCE = "source";
    const char* NAME = "name";
    const char* ID = "id";
    const char* GID = "gid";
    const char* X = "x";
    const char* Y = "y";
    const char* WIDTH = "width";
    const char* HEIGHT = "height";
    const char* TILE_WIDTH = "tilewidth";
    const char* TILE_HEIGHT = "tileheight";
    const char* FIRST_GID = "firstgid";
    const std::string TILE = "tile";
    const std::string IMAGE = "image";
    const std::string FRAME = "frame";
    const std::string OBJECT = "object";
    const std::string OBJECT_GROUP = "objectgroup";
    const std::string TILESET = "tileset";
    const std::string LAYER = "layer";
    const std::string ANIMATION = "animation";
    const std::string EMPTY = "";

    std::string GetStringAttribute(tinyxml2::XMLElement* p_elem,
                                   const char* p_attr)
    {
        const char* attrVal = p_elem->Attribute(p_attr);
        return attrVal != nullptr ? std::string(attrVal) : EMPTY;
    }

    void ReadObjectGroupElement(tinyxml2::XMLElement* p_pObjGr,
                                pp::tiled::ObjectGroup& p_rObjGr)
    {
        p_rObjGr.m_name = GetStringAttribute(p_pObjGr, NAME);
        tinyxml2::XMLElement* elem = p_pObjGr->FirstChildElement();
        while (elem != nullptr) {
            const char* elemName = elem->Name();
            if (OBJECT == elemName) {
                pp::tiled::Object obj;
                obj.m_id = elem->IntAttribute(ID);
                obj.m_gid = elem->IntAttribute(GID);
                obj.m_x = elem->IntAttribute(X);
                obj.m_y = elem->IntAttribute(Y);
                obj.m_w = elem->IntAttribute(WIDTH);
                obj.m_h = elem->IntAttribute(HEIGHT);
                obj.m_type = GetStringAttribute(p_pObjGr, NAME);
                obj.m_name = GetStringAttribute(p_pObjGr, TYPE);
                p_rObjGr.m_objects.push_back(obj);
            }
            elem = elem->NextSiblingElement();
        }
    }

    void ReadAnimationElement(tinyxml2::XMLElement* p_pAnim,
                              pp::tiled::Animation& p_rAnim)
    {
        tinyxml2::XMLElement* elem = p_pAnim->FirstChildElement();
        while (elem != nullptr) {
            const char* elemName = elem->Name();
            if (FRAME == elemName) {
                pp::tiled::Frame frame;
                frame.m_tileId = elem->IntAttribute(TILEID);
                p_rAnim.m_frames.push_back(frame);
            }
            elem = elem->NextSiblingElement();
        }
    }

    void ReadTileElement(tinyxml2::XMLElement* p_pTile,
                         pp::tiled::Tile& p_rTile)
    {
        p_rTile.m_id = p_pTile->IntAttribute(ID);
        p_rTile.m_type = GetStringAttribute(p_pTile, TYPE);

        tinyxml2::XMLElement* elem = p_pTile->FirstChildElement();
        while (elem != nullptr) {
            const char* elemName = elem->Name();

            if (ANIMATION == elemName) {
                ReadAnimationElement(elem, p_rTile.m_animation);
            } else if (OBJECT_GROUP == elemName) {
                ReadObjectGroupElement(elem, p_rTile.m_objects);
            }
            elem = elem->NextSiblingElement();
        }
    }

    void ReadLayerElement(tinyxml2::XMLElement* p_pLayer,
                          pp::tiled::Layer& p_rLayer)
    {
        p_rLayer.m_name = GetStringAttribute(p_pLayer, NAME);
        p_rLayer.m_width = p_pLayer->IntAttribute(WIDTH);
        p_rLayer.m_height = p_pLayer->IntAttribute(HEIGHT);
        std::string content = p_pLayer->FirstChildElement()->GetText();

        p_rLayer.m_data.clear();
        size_t contentPos = 0;
        for (int i=0; i<p_rLayer.m_height; i++) {

            p_rLayer.m_data.push_back(std::vector<int>());
            for (int j=0; j<p_rLayer.m_width; j++) {
                size_t nbpos = content.find_first_of(NUMBERS, contentPos);
                contentPos = content.find_first_not_of(NUMBERS, nbpos);
                int tile = std::stoi(content.substr(nbpos, contentPos-nbpos));
                p_rLayer.m_data[i].push_back(tile);
            }
        }
    }
}

namespace pp::tiled
{

    bool TileSet::Init(const std::string& p_filename)
    {
        bool success = true;

        tinyxml2::XMLDocument doc;
        tinyxml2::XMLError err = doc.LoadFile(p_filename.c_str());
        success = (err == tinyxml2::XML_SUCCESS);
        if (success) {
            tinyxml2::XMLElement* root = doc.RootElement();

            m_name = root->Attribute(NAME);
            m_width = root->IntAttribute(TILE_WIDTH);
            m_height = root->IntAttribute(TILE_HEIGHT);
            m_count = root->IntAttribute("tilecount");
            m_columns = root->IntAttribute("columns");

            tinyxml2::XMLElement* elem = root->FirstChildElement();
            while(elem != nullptr) {
                const char* elemName = elem->Name();
                if (IMAGE == elemName) {
                    m_image = GetStringAttribute(elem, SOURCE);

                } else if (TILE == elemName) {
                    pp::tiled::Tile tile;
                    ReadTileElement(elem, tile);
                    m_tiles[tile.m_id] = tile;
                }

                elem = elem->NextSiblingElement();
            }

        }

        return success;
    }

    bool TileMap::Init(const std::string& p_name)
    {

        tinyxml2::XMLDocument doc;
        tinyxml2::XMLError err = doc.LoadFile(p_name.c_str());
        bool success = err == tinyxml2::XML_SUCCESS;
        if (success) {
            // Read xml
            tinyxml2::XMLElement* root = doc.RootElement();

            m_heigth = root->IntAttribute(HEIGHT);
            m_width = root->IntAttribute(WIDTH);
            m_tileheight = root->IntAttribute(TILE_HEIGHT);
            m_tilewidth = root->IntAttribute(TILE_WIDTH);

            tinyxml2::XMLElement* elem = root->FirstChildElement();
            while (elem != nullptr) {
                const char* elemName = elem->Name();

                if (TILESET == elemName) {
                    TilesetRef tileset;
                    tileset.m_firstGid = elem->IntAttribute(FIRST_GID);
                    tileset.m_source = GetStringAttribute(elem, SOURCE);
                    m_tilesets.push_back(tileset);
                }
                else if (LAYER == elemName) {
                    Layer layer;
                    ReadLayerElement(elem, layer);
                    m_layers[layer.m_name] = layer;
                }
                else if (OBJECT_GROUP == elemName) {
                    ObjectGroup objgr;
                    ReadObjectGroupElement(elem, objgr);
                    m_objects[objgr.m_name] = objgr;
                }
                elem = elem->NextSiblingElement();
            }
        }

        return success;
    }

    const TilesetRef& TileMap::FindTilesetRef(int p_Gid) const {
        std::vector<TilesetRef>::const_iterator end = m_tilesets.end();
        std::vector<TilesetRef>::const_iterator it = m_tilesets.begin();
        std::vector<TilesetRef>::const_iterator found = m_tilesets.end();
        int max = 0;
        for(; it != end; ++it) {
            if (it->m_firstGid <= p_Gid && it->m_firstGid >= max) {
                found = it;
            }
        }
        return *found;
    }

}


