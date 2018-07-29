#include "Tiled.h"
#include "tinyxml2.h"
#include "RessourcesRepo.h"

namespace {

    const char* NUMBERS = "0123456789";
    const char* BACKGROUND = "Background";
    const char* WALLS = "Walls";
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

TileMap::TileMap()
: m_tilesets(),
  m_layers(),
  m_width(0),
  m_heigth(0),
  m_tilewidth(0),
  m_tileheight(0),
  m_pWindow(nullptr),
  m_background(nullptr),
  m_tiles(nullptr),
  m_x(),
  m_y()
{

}
TileMap::~TileMap()
{
    if (m_tiles == nullptr) return;
    for (int i=0; i<m_heigth; i++)
    {
        delete[] m_tiles[i];
    }
    delete[] m_tiles;
    m_tiles = nullptr;
}

TileMap::TileMap(const TileMap& p_tileMap)
: m_tilesets(p_tileMap.m_tilesets),
  m_layers(p_tileMap.m_layers),
  m_width(p_tileMap.m_width),
  m_heigth(p_tileMap.m_heigth),
  m_tilewidth(p_tileMap.m_tilewidth),
  m_tileheight(p_tileMap.m_tileheight),
  m_pWindow(p_tileMap.m_pWindow),
  m_background(p_tileMap.m_background),
  m_tiles(nullptr),
  m_x(p_tileMap.m_x),
  m_y(p_tileMap.m_y)
{
    if (m_heigth > 0) {
        m_tiles = new Tile*[m_heigth];
        for (int i=0; i<m_heigth; i++)
        {
            m_tiles[i] = new Tile[m_width];
            memcpy(m_tiles[i], p_tileMap.m_tiles[i], m_width*sizeof(Tile));
        }
    }
}
TileMap::TileMap(TileMap&& p_tileMap)
: m_tilesets(std::move(p_tileMap.m_tilesets)),
  m_layers(std::move(p_tileMap.m_layers)),
  m_width(p_tileMap.m_width),
  m_heigth(p_tileMap.m_heigth),
  m_tilewidth(p_tileMap.m_tilewidth),
  m_tileheight(p_tileMap.m_tileheight),
  m_pWindow(p_tileMap.m_pWindow),
  m_background(p_tileMap.m_background),
  m_tiles(nullptr),
  m_x(p_tileMap.m_x),
  m_y(p_tileMap.m_y)
{
    p_tileMap.m_tiles = nullptr;
    p_tileMap.m_width = 0;
    p_tileMap.m_heigth = 0;
}

TileMap& TileMap::operator=(TileMap p_tileMap)
{
    std::swap(m_tilesets, p_tileMap.m_tilesets);
    std::swap(m_layers, p_tileMap.m_layers);
    std::swap(m_width, p_tileMap.m_width);
    std::swap(m_heigth, p_tileMap.m_heigth);
    std::swap(m_tilewidth, p_tileMap.m_tilewidth);
    std::swap(m_tileheight, p_tileMap.m_tileheight);
    std::swap(m_pWindow, p_tileMap.m_pWindow);
    std::swap(m_background, p_tileMap.m_background);
    std::swap(m_tiles, p_tileMap.m_tiles);
    std::swap(m_x, p_tileMap.m_x);
    std::swap(m_y, p_tileMap.m_y);

    return *this;
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

bool TileMap::Init(const LWindow& p_window, const RessourcesRepo& p_ressourceRepo, const std::string& p_filename)
{
    bool success = true;

    m_pWindow = &p_window;

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError err = doc.LoadFile(p_filename.c_str());
    if (err == tinyxml2::XML_SUCCESS)
    {
        // Read xml
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
                int width = elem->IntAttribute("width");
                int heigth = elem->IntAttribute("height");
                std::string content = elem->FirstChildElement()->GetText();
                LayerNode l(name, width, heigth, content);
                m_layers.push_back(l);
            }
            elem = elem->NextSiblingElement();
        }
    }
    else
    {
        success = false;
    }

    // Find background and walls tileset among tileset read
    const TilesetNode* backgroundTileset = nullptr;
    const TilesetNode* wallsTileset = nullptr;
    const LayerNode* backgroundNode = nullptr;
    const LayerNode* wallsNode = nullptr;
    if (success) {
        for (const LayerNode& layer : m_layers) {
            if (layer.m_name == BACKGROUND) {
                backgroundNode = &layer;
                backgroundTileset = &FindTileset(layer.m_tiles[0][0]);
            } else if (layer.m_name == WALLS) {
                wallsNode = &layer;
                wallsTileset = &FindTileset(layer.m_tiles[0][0]);
            }
        }
    }
    success |= (backgroundNode != nullptr && wallsNode != nullptr);

    // Initialize tiles of map
    if (success) {
        m_background = &backgroundTileset->m_tileSet.m_image;
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
                    m_tiles[i][j].m_blocked = (wallsNode->m_tiles[i][j] - wallsTileset->m_firstGid == 3);
                }
            }
        }
    }

    return success;
}

void TileMap::Render() {
    int x = m_x;
    int y = m_y;

    int wh = m_pWindow->getHeight();
    int ww = m_pWindow->getWidth();

    int leftmost = x > 0 ? x/m_tilewidth : 0;
    int rightmost = (x + ww)/m_tilewidth + 1 < m_width ? (x + ww)/m_tilewidth + 1 : m_width;

    int upmost = y > 0 ? y/m_tileheight : 0;
    int downmost = (y + wh)/m_tileheight + 1 < m_heigth ? (y + wh)/m_tileheight + 1 : m_heigth;

    for (int i = upmost; i < downmost; i++)
    {
        for (int j = leftmost; j < rightmost; j++)
        {
            int tilePosX = j*m_tilewidth - x;
            int tilePosY = i*m_tileheight - y;
            m_background->render(tilePosX, tilePosY, &m_tiles[i][j].m_rect);
        }
    }
}

void TileMap::Update(int x, int y) {
    m_x = x;
    m_y = y;
}

bool TileMap::IsBlocked(int x, int y)
{
    // Effective position in the room
    int ePosX = x + m_x;
    int ePosY = y + m_y;

    // Index of tile
    int ix = ePosX/m_tilewidth;
    int iy = ePosY/m_tileheight;

    return !(ix >= 0 && ix < m_width && iy >= 0 && iy < m_heigth) ||
           m_tiles[iy][ix].m_blocked;
}

/*************************************************************************************
 struct TileSetNode
*************************************************************************************/

TileMap::TilesetNode::TilesetNode(int p_firstGid, const TileSet& p_tileSet)
: m_firstGid(p_firstGid),
  m_tileSet(p_tileSet)
{
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


