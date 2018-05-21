#include "FakeLevelState.h"

FakeLevelState::FakeLevelState()
: m_FakeBackground(),
  m_PetitPoint()
{
    //ctor
}

FakeLevelState::~FakeLevelState()
{
    //dtor
}

bool FakeLevelState::Init(const LWindow& p_pWindow)
{
    bool success = true;

    if (success)
    {
        success = m_FakeBackground.Load(p_pWindow);
    }

    if (success)
    {
        success = m_PetitPoint.Load(p_pWindow);
    }

    if (success)
    {
        m_PetitPoint.Init(p_pWindow.getWidth()/2 - m_PetitPoint.getWidth()/2,
                          p_pWindow.getHeight()/2 - m_PetitPoint.getHeight()/2);
        m_FakeBackground.Init(0,0);
    }

    return success;
}
GameState* FakeLevelState::Update(const SDL_Event& e)
{
    int x = m_FakeBackground.getX();
    int y = m_FakeBackground.getY();
    bool moved = false;

    //User presses a key
    if( e.type == SDL_KEYDOWN )
    {
        int ppx = m_PetitPoint.getX();
        int ppy = m_PetitPoint.getY();
        int ppw = m_PetitPoint.getWidth();
        int pph = m_PetitPoint.getHeight();

        //Select surfaces based on key press
        switch( e.key.keysym.sym )
        {
            case SDLK_UP:
                if (m_FakeBackground.CanMoveThere(ppx,ppy-2,ppw,pph))
                {
                    y-=2;
                    moved = true;
                }
                else if (m_FakeBackground.CanMoveThere(ppx,ppy-1,ppw,pph))
                {
                    y-=1;
                    moved = true;
                }
                break;
            case SDLK_DOWN:
                if (m_FakeBackground.CanMoveThere(ppx,ppy+2,ppw,pph))
                {
                    y+=2;
                    moved = true;
                }
                else if (m_FakeBackground.CanMoveThere(ppx,ppy+1,ppw,pph))
                {
                    y+=1;
                    moved = true;
                }
                break;
            case SDLK_LEFT:
                if (m_FakeBackground.CanMoveThere(ppx-2,ppy,ppw,pph))
                {
                    x-=2;
                    moved = true;
                }
                else if (m_FakeBackground.CanMoveThere(ppx-1,ppy,ppw,pph))
                {
                    x-=1;
                    moved = true;
                }
                break;
            case SDLK_RIGHT:
                if (m_FakeBackground.CanMoveThere(ppx+2,ppy,ppw,pph))
                {
                    x+=2;
                    moved = true;
                }
                else if (m_FakeBackground.CanMoveThere(ppx+1,ppy,ppw,pph))
                {
                    x+=1;
                    moved = true;
                }
                break;
            default:
                break;
        }
    }

    m_FakeBackground.Update(x,y);
    m_PetitPoint.Update(moved);

    return this;
}
void FakeLevelState::Render()
{
    m_FakeBackground.Render();
    m_PetitPoint.Render();
}
