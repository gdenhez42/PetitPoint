#include "Room.h"
#include "Tile.h"
#include "LWindow.h"
#include <fstream>
#include <stdlib.h>

Room::Room()
: m_height(-1),
  m_width(-1)
{
    //ctor
}

Room::~Room()
{
    //dtor
}

bool Room::Load(const LWindow& window, const RessourcesRepo& resources, const std::string& p_roomPath)
{

    m_pWindow = &window;
    std::ifstream f(p_roomPath);
    if (f)
    {
        char buf[11];
        f.getline(buf,11);
        m_width = atoi(buf);
        f.getline(buf,11);
        m_height = atoi(buf);

        m_tiles = new Tile*[m_height];
        for (int i=0; i<m_height; i++)
        {
            m_tiles[i] = new Tile[m_width];
            for(int j=0; j<m_width; j++)
            {
                char tileCode = f.get();
                switch (tileCode)
                {
                case 'W':
                    m_tiles[i][j].Init(window, resources, Tile::WALL);
                    break;
                default:
                    m_tiles[i][j].Init(window, resources, Tile::NONE);
                    break;
                }
            }

            // Get the end of the line
            f.get();
        }

        f.close();
        return true;
    }
    return false;
}
/*******************************************************
Update the position of the camera in the room

Parameters:
  - x : the position of the left corner of the screen
        relative to the position of the left-most tile
  - y : the position of the up corner of the screen
        relative to the position of the up-most tile
*******************************************************/
void Room::Update(int x, int y)
{
    m_x = x;
    m_y = y;
}

void Room::Render()
{
    int x = m_x;
    int y = m_y;

    int wh = m_pWindow->getHeight();
    int ww = m_pWindow->getWidth();

    int leftmost = x > 0 ? x/Tile::SIZE : 0;
    int rightmost = (x + ww)/Tile::SIZE + 1 < m_width ? (x + ww)/Tile::SIZE + 1 : m_width;

    int upmost = y > 0 ? y/Tile::SIZE : 0;
    int downmost = (y + wh)/Tile::SIZE + 1 < m_height ? (y + wh)/Tile::SIZE + 1 : m_height;

    for (int i = upmost; i < downmost; i++)
    {
        for (int j = leftmost; j < rightmost; j++)
        {
            int tilePosX = j*Tile::SIZE - x;
            int tilePosY = i*Tile::SIZE - y;
            m_tiles[i][j].Render(tilePosX, tilePosY);
        }
    }
}

/************************************************************
Get the current tile at the specified position on the screen

************************************************************/
const Tile& Room::getTile(int x, int y) const
{
    // Effective position in the room
    int ePosX = x + m_x;
    int ePosY = y + m_y;

    return m_tiles[ePosY/Tile::SIZE][ePosX/Tile::SIZE];

}

void Room::Free()
{
    for (int i=0; i<m_height; i++)
    {
        delete m_tiles[i];
    }
    delete m_tiles;
    m_height = -1;
    m_width = -1;
}
