#include "FakeBackground.h"

namespace {
    enum Elements
    {
        NORMAL,
        MUR
    };
    const int TILE_SIZE = 32;
    const int FAKEBACKGROUND_W = 40;
    const int FAKEBACKGROUND_HE = 30;
}

FakeBackground::FakeBackground()
{
    //ctor
}

FakeBackground::~FakeBackground()
{
    //dtor
}

bool FakeBackground::Load(const LWindow& window)
{
    m_pRenderer = window.getRenderer();

    m_wx = window.getWidth();
    m_wy = window.getHeight();

    // Idea: load from file? from resource?
    for (int i = 0; i < FAKEBACKGROUND_W; i++)
    {
        for (int j = 0; j < FAKEBACKGROUND_HE; j++)
        {
            if (i == 0 || i == FAKEBACKGROUND_W-1 || j == 0 || j == FAKEBACKGROUND_HE-1)
            {
                m_area[i][j] = MUR;
            }
            else
            {
                m_area[i][j] = NORMAL;
            }
        }
    }
    return true;

}
void FakeBackground::Init(int x, int y)
{
    Update(x,y);
}
void FakeBackground::Update(int x, int y)
{
    m_x = x;
    m_y = y;
}
void FakeBackground::Render()
{
    int startX = 0;
    if (m_x >= 0)
    {
        startX = m_x / TILE_SIZE;
    }
    int startY = 0;
    if (m_y >=0)
    {
        startY = m_y / TILE_SIZE;
    }
    int endX = (m_x + m_wx) / TILE_SIZE + 1;
    if (endX >= FAKEBACKGROUND_W)
    {
        endX = FAKEBACKGROUND_W;
    }
    int endY = (m_y + m_wy) / TILE_SIZE + 1;
    if (endY >= FAKEBACKGROUND_HE)
    {
        endY = FAKEBACKGROUND_HE;
    }

    // render loop
    for (int x = startX; x < endX; x++)
    {
        for (int y = startY; y < endY; y++)
        {
            int renderX = x*TILE_SIZE - m_x;
            int renderY = y*TILE_SIZE - m_y;

            SDL_Rect fillRect = { renderX, renderY, TILE_SIZE, TILE_SIZE };
            if (m_area[x][y] == MUR)
            {
                SDL_SetRenderDrawColor(m_pRenderer, 0x80, 0x80, 0x80, 0xFF);
            }
            else if (m_area[x][y] == NORMAL)
            {
                SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
            SDL_RenderFillRect( m_pRenderer, &fillRect );
        }
    }

}

bool FakeBackground::CanMoveThere(int x, int y, int w, int h)
{
    bool canMove = true;
    int positionOnMapX = m_x + x;
    int positionOnMapY = m_y + y;

    // First check if off bound
    if (positionOnMapX < 0 || positionOnMapX+w > FAKEBACKGROUND_W*TILE_SIZE ||
        positionOnMapY < 0 || positionOnMapY+h > FAKEBACKGROUND_HE*TILE_SIZE)
    {
        canMove = false;
    }

    // Then check if in wall
    if (canMove)
    {
        int startX = positionOnMapX / TILE_SIZE;
        int startY = positionOnMapY / TILE_SIZE;
        int endX = (positionOnMapX+w) / TILE_SIZE + 1;
        int endY = (positionOnMapY+h) / TILE_SIZE + 1;

        for (int tx = startX; canMove && tx < endX; tx++)
        {
            for (int ty = startY; canMove && ty < endY; ty++)
            {
                canMove = m_area[tx][ty] == NORMAL;
            }
        }

    }

    return canMove;
}
