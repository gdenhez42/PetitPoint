#include "Tiled.h"
#include "tinyxml2.h"
#include "RessourcesRepo.h"

namespace {

    const char* NUMBERS = "0123456789";
    const char* ANIMATION = "animation";
    const char* TILEID = "tileid";
    const char* TYPE = "type";
    const std::string TILE = "tile";
    const std::string EMPTY = "";
}

namespace pp
{

    /*************************************************************************************
 class TileSet
    *************************************************************************************/

    TileSet::TileSet()
    {
        //ctor
    }

    TileSet::~TileSet()
    {
        //dtor
    }


    bool TileSet::Init(const RessourcesRepo& p_ressourceRepo, const std::string& p_filename)
    {
        bool success = true;

        tinyxml2::XMLDocument doc;
        tinyxml2::XMLError err = doc.LoadFile(p_filename.c_str());
        success = (err == tinyxml2::XML_SUCCESS);
        if (success)
            {
                tinyxml2::XMLElement* root = doc.RootElement();

                m_name = root->Attribute("name");
                m_width = root->IntAttribute("tilewidth");
                m_height = root->IntAttribute("tileheight");
                m_count = root->IntAttribute("tilecount");
                m_columns = root->IntAttribute("columns");

                tinyxml2::XMLElement* image = root->FirstChildElement("image");

                std::string imagePath = image->Attribute("source");
                m_image = &p_ressourceRepo.getImage(
                                                    RessourcesRepo::getRessourceName(imagePath));
            }

        if (success) {
            tinyxml2::XMLElement* root = doc.RootElement();
            tinyxml2::XMLElement* elem = root->FirstChildElement();
            while(elem != nullptr) {
                if (TILE == elem->Name()) {
                    int id = elem->IntAttribute("id");
                    tinyxml2::XMLElement* anim = elem->FirstChildElement(ANIMATION);
                    if (anim != nullptr) {
                        LAnimation animation;
                        tinyxml2::XMLElement* frame = anim->FirstChildElement();
                        while (frame != nullptr) {
                            int tileid = frame->IntAttribute(TILEID);
                            animation.AddFrame(LAnimation::Frame(m_image,
                                                                 {(tileid%m_columns)*m_width, (tileid/m_columns)*m_height, m_width, m_height}));
                            frame = frame->NextSiblingElement();
                        }

                        m_animations[id] = animation;
                    }

                    const char* typeAttr = elem->Attribute(TYPE);
                    m_types[id] = typeAttr == nullptr ? EMPTY : typeAttr;
                }

                elem = elem->NextSiblingElement();
            }

        }

        return success;
    }

    const LAnimation& TileSet::getAnimation(int id) const
    {
        return m_animations.at(id);
    }

    const std::string& TileSet::getType(int id) const
    {
        std::map<int, std::string>::const_iterator it = m_types.find(id);
        return it != m_types.end() ? it->second : EMPTY;
    }

    /*************************************************************************************
 class TileMap
    *************************************************************************************/

    TileMap::TileMap()
        : m_tilesets(),
          m_layers(),
          m_width(0),
          m_heigth(0),
          m_tilewidth(0),
          m_tileheight(0)
    {

    }
    TileMap::~TileMap()
    {

    }

    TileMap::TileMap(const TileMap& p_tileMap)
        : m_tilesets(p_tileMap.m_tilesets),
          m_layers(p_tileMap.m_layers),
          m_width(p_tileMap.m_width),
          m_heigth(p_tileMap.m_heigth),
          m_tilewidth(p_tileMap.m_tilewidth),
          m_tileheight(p_tileMap.m_tileheight)
    {
    }

    TileMap::TileMap(TileMap&& p_tileMap)
        : m_tilesets(std::move(p_tileMap.m_tilesets)),
          m_layers(std::move(p_tileMap.m_layers)),
          m_width(p_tileMap.m_width),
          m_heigth(p_tileMap.m_heigth),
          m_tilewidth(p_tileMap.m_tilewidth),
          m_tileheight(p_tileMap.m_tileheight)
    {
    }

    TileMap& TileMap::operator=(TileMap p_tileMap)
    {
        std::swap(m_tilesets, p_tileMap.m_tilesets);
        std::swap(m_layers, p_tileMap.m_layers);
        std::swap(m_width, p_tileMap.m_width);
        std::swap(m_heigth, p_tileMap.m_heigth);
        std::swap(m_tilewidth, p_tileMap.m_tilewidth);
        std::swap(m_tileheight, p_tileMap.m_tileheight);

        return *this;
    }

    bool TileMap::Init(const RessourcesRepo& p_ressourceRepo, const std::string& p_name)
    {
        bool success = true;

        tinyxml2::XMLDocument doc;
        tinyxml2::XMLError err = doc.LoadFile(p_name.c_str());
        if (err == tinyxml2::XML_SUCCESS)
            {
                // filename anname are the same for simplicity
                m_name = RessourcesRepo::getRessourceName(p_name);

                // Read xml
                tinyxml2::XMLElement* root = doc.RootElement();
                tinyxml2::XMLElement* elem = root->FirstChildElement();

                m_heigth = root->IntAttribute("height");
                m_width = root->IntAttribute("width");
                m_tileheight = root->IntAttribute("tileheight");
                m_tilewidth = root->IntAttribute("tilewidth");

                while (elem != nullptr) {
                    if (strcmp(elem->Name(), "tileset") == 0) {
                        std::string source = RessourcesRepo::getRessourceName(elem->Attribute("source"));
                        int firstgid = elem->IntAttribute("firstgid");
                        TilesetNode tsn(firstgid,
                                        p_ressourceRepo.getTileSet(source));
                        m_tilesets.push_back(tsn);
                    }
                    else if (strcmp(elem->Name(), "layer") == 0) {
                        std::string name = elem->Attribute("name");
                        int width = elem->IntAttribute("width");
                        int heigth = elem->IntAttribute("height");
                        std::string content = elem->FirstChildElement()->GetText();
                        LayerNode l(name, width, heigth, content);
                        m_layers.push_back(l);
                    }
                    else if (strcmp(elem->Name(), "objectgroup") == 0) {
                        ObjectGroup objgr;
                        objgr.m_name = elem->Attribute("name");
                        tinyxml2::XMLElement* child = elem->FirstChildElement();
                        while (child != nullptr) {
                            Object obj;
                            obj.m_id = child->IntAttribute("id");
                            obj.m_x = child->IntAttribute("x");
                            obj.m_y = child->IntAttribute("y");
                            obj.m_w = child->IntAttribute("width");
                            obj.m_h = child->IntAttribute("height");
                            obj.m_type = child->Attribute("type");
                            tinyxml2::XMLElement* prop = child->FirstChildElement()->FirstChildElement();
                            while (prop != nullptr) {
                                obj.m_properties[prop->Attribute("name")] = prop->Attribute("value");
                                prop  = prop->NextSiblingElement();
                            }
                            objgr.m_objects.push_back(obj);
                            child = child->NextSiblingElement();
                        }
                        m_objects.push_back(objgr);
                    }
                    elem = elem->NextSiblingElement();
                }
            }
        else
            {
                success = false;
            }

        return success;
    }

    const TileMap::TilesetNode& TileMap::FindTileset(int gid) const
    {
        std::vector<TilesetNode>::const_iterator t = m_tilesets.begin();
        std::vector<TilesetNode>::const_iterator it = m_tilesets.begin();
        std::vector<TilesetNode>::const_iterator end = m_tilesets.end();
        for (; it != end; ++it)
            {
                if (it->m_firstGid <= gid && it->m_firstGid > t->m_firstGid) {
                    t = it;
                }
            }
        return *t;
    }

    bool TileMap::FindLayerNode(const std::string& p_name,
                                const LayerNode** p_ppLayer) const
    {
        bool found = false;
        std::vector<LayerNode>::const_iterator it, itend = m_layers.end();
        for (it = m_layers.begin(); it != itend && !found; ++it) {
            if (it->m_name == p_name) {
                (*p_ppLayer) = &(*it);
                found = true;
            }
        }
        return found;
    }

    bool TileMap::FindLayerNode(const std::string& p_name,
                                const LayerNode** p_ppLayer,
                                const TilesetNode** p_ppTileset) const
    {
        bool found = false;
        std::vector<LayerNode>::const_iterator it, itend = m_layers.end();
        for (it = m_layers.begin(); it != itend && !found; ++it) {
            if (it->m_name == p_name) {
                (*p_ppLayer) = &(*it);
                (*p_ppTileset) = &FindTileset(it->m_tiles[0][0]);
                found = true;
            }
        }
        return found;
    }

    bool TileMap::FindObjectGroup(const std::string& p_name, const ObjectGroup** p_ppObj) const
    {
        bool found = false;
        std::vector<ObjectGroup>::const_iterator it, itend = m_objects.end();
        for (it = m_objects.begin(); it != itend && !found; ++it) {
            if (it->m_name == p_name) {
                (*p_ppObj) = &(*it);
                found = true;
            }
        }
        return found;
    }

    /*************************************************************************************
 struct TileSetNode
    *************************************************************************************/

    TileMap::TilesetNode::TilesetNode(int p_firstGid, const TileSet& p_tileSet)
        : m_firstGid(p_firstGid),
          m_tileSet(p_tileSet)
    {
    }

    const LTexture* TileMap::TilesetNode::GetTexture() const
    {
        return m_tileSet.m_image;
    }


    /*************************************************************************************
 struct LayerNode
    *************************************************************************************/
    TileMap::LayerNode::LayerNode(const std::string& name, int width, int heigth, const std::string& p_content)
        : m_tiles(nullptr),
          m_name(name),
          m_width(width),
          m_heigth(heigth)
    {
        m_tiles = new int*[heigth];

        size_t contentPos = 0;
        for (int i=0; i<heigth; i++)
            {
                m_tiles[i] = new int[width];

                for (int j=0; j<width; j++)
                    {
                        size_t nbpos = p_content.find_first_of(NUMBERS, contentPos);
                        contentPos = p_content.find_first_not_of(NUMBERS, nbpos);
                        int tile = std::stoi(p_content.substr(nbpos, contentPos-nbpos));
                        m_tiles[i][j] = tile;
                    }
            }
    }

    TileMap::LayerNode::~LayerNode()
    {
        if (m_tiles == nullptr) return;
        for (int i=0; i<m_heigth; i++)
            {
                delete[] m_tiles[i];
            }
        delete[] m_tiles;
        m_tiles = nullptr;
    }

    TileMap::LayerNode::LayerNode(const LayerNode& p_layerNode)
        : m_tiles(nullptr),
          m_name(p_layerNode.m_name),
          m_width(p_layerNode.m_width),
          m_heigth(p_layerNode.m_heigth)
    {
        if (m_heigth > 0) {
            m_tiles = new int*[m_heigth];
            for (int i=0; i<m_heigth; i++)
                {
                    m_tiles[i] = new int[m_width];
                    memcpy(m_tiles[i], p_layerNode.m_tiles[i], m_width*sizeof(int));
                }
        }
    }
    TileMap::LayerNode::LayerNode(LayerNode&& p_layerNode)
        : m_name(std::move(p_layerNode.m_name)),
          m_width(p_layerNode.m_width),
          m_heigth(p_layerNode.m_heigth)
    {
        m_tiles = p_layerNode.m_tiles;
        p_layerNode.m_heigth = 0;
        p_layerNode.m_width = 0;
        p_layerNode.m_tiles = nullptr;
    }

    TileMap::LayerNode& TileMap::LayerNode::operator=(LayerNode p_layerNode)
    {
        std::swap(p_layerNode.m_tiles, m_tiles);
        m_width = p_layerNode.m_width;
        m_heigth = p_layerNode.m_heigth;
        return *this;
    }

}


