#include "Tile.h"
#include "LWindow.h"
#include "RessourcesRepo.h"
#include <SDL2/SDL.h>

const int Tile::SIZE = 64;


Tile::Tile()
{
    //ctor
}

Tile::~Tile()
{
    //dtor
}


bool Tile::Init(const LWindow& window, const RessourcesRepo& resources, Kind tileKind)
{
    m_kind = tileKind;
    m_pRenderer = window.getRenderer();

    switch (m_kind)
    {
    case NONE:
        m_isBlocked = false;
        m_isWarp = false;
        break;
    case FLOOR:
        m_isBlocked = false;
        m_isWarp = false;
        break;
    case WALL:
        m_isBlocked = true;
        m_isWarp = false;
        break;
    case WINDOW:
        m_isBlocked = true;
        m_isWarp = false;
        break;
    case DOOR:
        m_isBlocked = false;
        m_isWarp = true;
        break;
    }

    return true;
}

/*****************************************************************************
Renders the tile

Parameters:
  - x : the position on the screen to render the tile
  - y : the position on the screnn to render the tile
******************************************************************************/
void Tile::Render(int x, int y)
{
    SDL_Rect rect = {x,y,SIZE,SIZE};
    switch (m_kind)
    {
    case NONE:
        SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(m_pRenderer, &rect);
        break;
    case FLOOR:
        SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(m_pRenderer, &rect);
        break;
    case WALL:
        SDL_SetRenderDrawColor(m_pRenderer, 0x80, 0x80, 0x80, 0xFF);
        SDL_RenderFillRect(m_pRenderer, &rect);
        break;
    case WINDOW:
        SDL_SetRenderDrawColor(m_pRenderer, 0xe0, 0xFF, 0xFB, 0xFF);
        SDL_RenderFillRect(m_pRenderer, &rect);
        break;
    case DOOR:
        SDL_SetRenderDrawColor(m_pRenderer, 0x7a, 0x4e, 0x1e, 0xFF);
        SDL_RenderFillRect(m_pRenderer, &rect);
        break;

    }
}
