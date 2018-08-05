#include "LevelState.h"
#include "LMap.h"

namespace pp {

LevelState::LevelState()
: m_currentRoom(nullptr),
  m_PetitPoint()
{
    //ctor
}

LevelState::~LevelState()
{
    //dtor
}

bool LevelState::Init(const LWindow& p_pWindow, const RessourcesRepo& p_ressourceRepo)
{
    bool success = true;

    success = m_maps["Manoir2_SJ"].Init(p_pWindow, p_ressourceRepo, "Manoir2_SJ.tmx");

    if (success)
    {
        m_currentRoom = &m_maps["Manoir2_SJ"];

        m_currentRoom->Update(0,0);
        m_PetitPoint.Init(p_ressourceRepo,
                          p_pWindow.getWidth()/2 - m_PetitPoint.getWidth()/2,
                          p_pWindow.getHeight()/2 - m_PetitPoint.getHeight()/2);
    }

    return success;
}

GameState* LevelState::Update(const SDL_Event& e)
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
void LevelState::Render()
{
    m_currentRoom->Render();
    m_PetitPoint.Render();
}

/***********************************************************
There must be a better way to do the walking part :-(
***********************************************************/

void LevelState::MovePetitPointLeft()
{
    int ppx = m_PetitPoint.getX();
    int ppy = m_PetitPoint.getY();
    int pph = m_PetitPoint.getHeight();

    int dx = 0;

    bool blocked = false;
    int toMove = PetitPoint::WALK_SPEED;

    while (toMove > 0 && !blocked)
    {
        if (m_currentRoom->isBlocked(ppx-dx-1, ppy) || m_currentRoom->isBlocked(ppx-dx-1, ppy+pph))
        {
            blocked = true;
        }
        else
        {
            dx++;
            toMove--;
        }
    }
    m_currentRoom->Update(m_currentRoom->getX()-dx,m_currentRoom->getY());
}
void LevelState::MovePetitPointRight()
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
        if (m_currentRoom->isBlocked(ppx+ppw+dx+1, ppy) || m_currentRoom->isBlocked(ppx+ppw+dx+1, ppy+pph))
        {
            blocked = true;
        }
        else
        {
            dx++;
            toMove--;
        }
    }
    m_currentRoom->Update(m_currentRoom->getX()+dx,m_currentRoom->getY());
}
void LevelState::MovePetitPointUp()
{
    int ppx = m_PetitPoint.getX();
    int ppy = m_PetitPoint.getY();
    int ppw = m_PetitPoint.getWidth();

    int dy = 0;

    bool blocked = false;
    int toMove = PetitPoint::WALK_SPEED;

    while (toMove > 0 && !blocked)
    {
        if (m_currentRoom->isBlocked(ppx, ppy-dy-1) || m_currentRoom->isBlocked(ppx+ppw, ppy-dy-1))
        {
            blocked = true;
        }
        else
        {
            dy++;
            toMove--;
        }
    }
    m_currentRoom->Update(m_currentRoom->getX(),m_currentRoom->getY()-dy);
}
void LevelState::MovePetitPointDown()
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
        if (m_currentRoom->isBlocked(ppx, ppy+pph+dy+1) || m_currentRoom->isBlocked(ppx+ppw, ppy+pph+dy+1))
        {
            blocked = true;
        }
        else
        {
            dy++;
            toMove--;
        }
    }
    m_currentRoom->Update(m_currentRoom->getX(),m_currentRoom->getY()+dy);
}

}
