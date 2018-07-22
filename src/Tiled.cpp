#include "Tiled.h"
#include "tinyxml2.h"
#include "RessourcesRepo.h"

namespace {

    const char* NUMBERS = "0123456789";
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


bool TileSet::Init(const LWindow& window, const std::string& p_filename)
{

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError err = doc.LoadFile(p_filename.c_str());
    if (err == tinyxml2::XML_SUCCESS)
    {
        tinyxml2::XMLElement* root = doc.RootElement();

        m_name = root->Attribute("name");
        m_width = root->IntAttribute("tilewidth");
        m_height = root->IntAttribute("tileheight");
        m_count = root->IntAttribute("tilecount");
        m_columns = root->IntAttribute("columns");

        tinyxml2::XMLElement* image = root->FirstChildElement("image");

        std::string imagePath = image->Attribute("source");
        return m_image.loadFromFile(window, std::string("Maps/")+imagePath);
    }

    return false;
}

/*************************************************************************************
 class TileMap
*************************************************************************************/

TileMap::TileMap() {
}
TileMap::~TileMap() {
}

bool TileMap::Init(const RessourcesRepo& p_ressourceRepo, const std::string& p_filename)
{
    bool success = true;

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError err = doc.LoadFile(p_filename.c_str());
    if (err == tinyxml2::XML_SUCCESS)
    {
        tinyxml2::XMLElement* root = doc.RootElement();
        tinyxml2::XMLElement* elem = root->FirstChildElement();

        m_heigth = root->IntAttribute("height");
        m_width = root->IntAttribute("width");
        m_tileheight = root->IntAttribute("tileheight");
        m_tilewidth = root->IntAttribute("tilewidth");

        while (elem != nullptr) {
            if (strcmp(elem->Name(), "tileset") == 0) {
                std::string source = elem->Attribute("source");
                int firstgid = elem->IntAttribute("firstgid");
                TilesetNode tsn(firstgid, p_ressourceRepo.getTileSet(source));
                m_tilesets.push_back(tsn);
            }
            else if (strcmp(elem->Name(), "layer") == 0) {
                std::string name = elem->Attribute("name");
                int heigth = elem->IntAttribute("width");
                int width = elem->IntAttribute("height");
                std::string content = elem->FirstChildElement()->GetText();
                LayerNode l(width, heigth, content);
                m_layers.insert(std::make_pair(name, l));
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

/*************************************************************************************
 class TileSetNode
*************************************************************************************/

TileMap::TilesetNode::TilesetNode(int p_firstGid, const TileSet& p_tileSet)
: m_firstGid(p_firstGid),
  m_tileSet(p_tileSet)
{
}


/*************************************************************************************
 class LayerNode
*************************************************************************************/
TileMap::LayerNode::LayerNode(int width, int heigth, const std::string& p_content)
: m_width(width),
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
}

TileMap::LayerNode::LayerNode(const LayerNode& p_layerNode)
: m_width(p_layerNode.m_width),
  m_heigth(p_layerNode.m_heigth)
{
    m_tiles = new int*[m_heigth];
    for (int i=0; i<m_heigth; i++)
    {
        m_tiles[i] = new int[m_width];
        memcpy(m_tiles[i], p_layerNode.m_tiles[i], m_width*sizeof(int));
    }
}
TileMap::LayerNode::LayerNode(LayerNode&& p_layerNode)
: m_width(p_layerNode.m_width),
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


