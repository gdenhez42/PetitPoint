#include "FakeLevelState.h"

namespace pp {

FakeLevelState::FakeLevelState()
: m_currentRoom(),
  m_PetitPoint()
{
    //ctor
}

FakeLevelState::~FakeLevelState()
{
    //dtor
}

bool FakeLevelState::Init(const LWindow& p_pWindow, const RessourcesRepo& p_ressourceRepo)
{
    bool success = true;

    m_currentRoom = p_ressourceRepo.getMap("Manoir_SJ.tmx");

    if (success)
    {
        m_currentRoom.Update(0,0);
        m_PetitPoint.Init(p_ressourceRepo,
                          p_pWindow.getWidth()/2 - m_PetitPoint.getWidth()/2,
                          p_pWindow.getHeight()/2 - m_PetitPoint.getHeight()/2);
    }

    return success;
}

GameState* FakeLevelState::Update(const SDL_Event& e)
{
    Command::Command command = Command::NONE;

    //User presses a key
    if( e.type == SDL_KEYDOWN )
    {
        //Select surfaces based on key press
        switch( e.key.keysym.sym )
        {
            case SDLK_UP:
                MovePetitPointUp();
                command = Command::MOVE_UP;
                break;
            case SDLK_DOWN:
                MovePetitPointDown();
                command = Command::MOVE_DOWN;
                break;
            case SDLK_LEFT:
                MovePetitPointLeft();
                command = Command::MOVE_LEFT;
                break;
            case SDLK_RIGHT:
                MovePetitPointRight();
                command = Command::MOVE_RIGHT;
                break;
            default:

                break;
        }
    }

    m_PetitPoint.Update(command);

    return this;
}
void FakeLevelState::Render()
{
    m_currentRoom.Render();
    m_PetitPoint.Render();
}

/***********************************************************
There must be a better way to do the walking part :-(
***********************************************************/

void FakeLevelState::MovePetitPointLeft()
{
    int ppx = m_PetitPoint.getX();
    int ppy = m_PetitPoint.getY();
    int pph = m_PetitPoint.getHeight();

    int dx = 0;

    bool blocked = false;
    int toMove = PetitPoint::WALK_SPEED;

    while (toMove > 0 && !blocked)
    {
        if (m_currentRoom.IsBlocked(ppx-dx-1, ppy) || m_currentRoom.IsBlocked(ppx-dx-1, ppy+pph))
        {
            blocked = true;
        }
        else
        {
            dx++;
            toMove--;
        }
    }
    m_currentRoom.Update(m_currentRoom.getX()-dx,m_currentRoom.getY());
}
void FakeLevelState::MovePetitPointRight()
{
    int ppx = m_PetitPoint.getX();
    int ppy = m_PetitPoint.getY();
    int ppw = m_PetitPoint.getWidth();
    int pph = m_PetitPoint.getHeight();

    int dx = 0;

    bool blocked = false;
    int toMove = PetitPoint::WALK_SPEED;

    while (toMove > 0 && !blocked)
    {
        if (m_currentRoom.IsBlocked(ppx+ppw+dx+1, ppy) || m_currentRoom.IsBlocked(ppx+ppw+dx+1, ppy+pph))
        {
            blocked = true;
        }
        else
        {
            dx++;
            toMove--;
        }
    }
    m_currentRoom.Update(m_currentRoom.getX()+dx,m_currentRoom.getY());
}
void FakeLevelState::MovePetitPointUp()
{
    int ppx = m_PetitPoint.getX();
    int ppy = m_PetitPoint.getY();
    int ppw = m_PetitPoint.getWidth();

    int dy = 0;

    bool blocked = false;
    int toMove = PetitPoint::WALK_SPEED;

    while (toMove > 0 && !blocked)
    {
        if (m_currentRoom.IsBlocked(ppx, ppy-dy-1) || m_currentRoom.IsBlocked(ppx+ppw, ppy-dy-1))
        {
            blocked = true;
        }
        else
        {
            dy++;
            toMove--;
        }
    }
    m_currentRoom.Update(m_currentRoom.getX(),m_currentRoom.getY()-dy);
}
void FakeLevelState::MovePetitPointDown()
{
    int ppx = m_PetitPoint.getX();
    int ppy = m_PetitPoint.getY();
    int ppw = m_PetitPoint.getWidth();
    int pph = m_PetitPoint.getHeight();

    int dy = 0;

    bool blocked = false;
    int toMove = PetitPoint::WALK_SPEED;

    while (toMove > 0 && !blocked)
    {
        if (m_currentRoom.IsBlocked(ppx, ppy+pph+dy+1) || m_currentRoom.IsBlocked(ppx+ppw, ppy+pph+dy+1))
        {
            blocked = true;
        }
        else
        {
            dy++;
            toMove--;
        }
    }
    m_currentRoom.Update(m_currentRoom.getX(),m_currentRoom.getY()+dy);
}

}
