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
    m_pWindow = &p_pWindow;

    // Get the maps of this level
    const TileMap& manoir2SJ = p_ressourceRepo.getMap("Manoir2_SJ.tmx");
    const TileMap& manoir2SA = p_ressourceRepo.getMap("Manoir2_SA.tmx");

    success = m_maps["Manoir2_SJ"].Init(p_pWindow, manoir2SJ, "Manoir2_SJ");
    success &= m_maps["Manoir2_SA"].Init(p_pWindow, manoir2SA, "Manoir2_SA");

    if (success)
    {
        m_currentRoom = &m_maps["Manoir2_SJ"];

        m_currentRoom->Update(0,0);
        m_PetitPoint.Init(p_ressourceRepo);
        m_PetitPoint.Warp(m_currentRoom->getName(), p_pWindow.getWidth()/2 - PetitPoint::IMAGE_SIZE/2,
                          p_pWindow.getHeight()/2 - PetitPoint::IMAGE_SIZE/2);

        std::map<std::string, LMap>::const_iterator it, itend = m_maps.end();
        for(it = m_maps.begin(); it != itend; ++it) {
            const std::vector<std::string>& warps = it->second.getLoads();
            std::vector<std::string>::const_iterator it2, it2end = warps.end();
            for (it2 = warps.begin(); it2 != it2end; ++it2) {
                m_loads[*it2] = it->first;
            }
        }
    }

    return success;
}

GameState* LevelState::Update(const SDL_Event& e, const Uint8* keyboardState)
{
    Command::Command command = Command::NONE;

    if (keyboardState[SDL_SCANCODE_UP]) {
        MovePetitPoint(UP);
        command = Command::MOVE_UP;
    }
    else if (keyboardState[SDL_SCANCODE_DOWN]) {
        MovePetitPoint(DOWN);
        command = Command::MOVE_DOWN;
    }
    else if (keyboardState[SDL_SCANCODE_LEFT]) {
        MovePetitPoint(LEFT);
        command = Command::MOVE_LEFT;
    }
    else if (keyboardState[SDL_SCANCODE_RIGHT]) {
        MovePetitPoint(RIGHT);
        command = Command::MOVE_RIGHT;
    }

    m_PetitPoint.Update(command);

    return this;
}
void LevelState::Render()
{
    m_currentRoom->Render();
    m_PetitPoint.Render(*m_currentRoom);
}

/***********************************************************
There must be a better way to do the walking part :-(
***********************************************************/
void LevelState::MovePetitPoint(Dir d)
{
    const Personage::HitBox& hb = m_PetitPoint.getGroundHb();

    bool inWarp = m_currentRoom->inWarp(m_PetitPoint.getX()+hb.m_x, m_PetitPoint.getY()+hb.m_y, hb.m_w, hb.m_h);
    int dx = 0;
    int dy = 0;
    int toMove = PetitPoint::WALK_SPEED;
    bool warped = false;
    std::string warp;

    while (toMove > 0)
    {
        int x1, y1, x2, y2;

        switch (d) {
            case LEFT:
                x1 = m_PetitPoint.getX()+hb.m_x+dx-1;
                y1 = m_PetitPoint.getY()+hb.m_y;
                x2 = m_PetitPoint.getX()+hb.m_x+dx-1;
                y2 = m_PetitPoint.getY()+hb.m_y+hb.m_h;
                break;
            case RIGHT:
                x1 = m_PetitPoint.getX()+hb.m_x+hb.m_w+dx+1;
                y1 = m_PetitPoint.getY()+hb.m_y;
                x2 = m_PetitPoint.getX()+hb.m_x+hb.m_w+dx+1;
                y2 = m_PetitPoint.getY()+hb.m_y+hb.m_h;
                break;
            case UP:
                x1 = m_PetitPoint.getX()+hb.m_x;
                y1 = m_PetitPoint.getY()+hb.m_y+dy-1;
                x2 = m_PetitPoint.getX()+hb.m_x+hb.m_w;
                y2 = m_PetitPoint.getY()+hb.m_y+dy-1;
                break;
            case DOWN:
                x1 = m_PetitPoint.getX()+hb.m_x;
                y1 = m_PetitPoint.getY()+hb.m_y+hb.m_h+dy+1;
                x2 = m_PetitPoint.getX()+hb.m_x+hb.m_w;
                y2 = m_PetitPoint.getY()+hb.m_y+hb.m_h+dy+1;
                break;
        }

        if (m_currentRoom->isBlocked(x1, y1) || m_currentRoom->isBlocked(x2, y2))
        {
            toMove = 0;
        }
        else if (!inWarp && (m_currentRoom->isWarp(x1, y1, warp) || m_currentRoom->isWarp(x2, y2, warp)))
        {
            toMove = 0;
            warped = true;
        }
        else
        {
            switch (d) {
                case LEFT:
                    dx--;
                    break;
                case RIGHT:
                    dx++;
                    break;
                case UP:
                    dy--;
                    break;
                case DOWN:
                    dy++;
                    break;
            }
            toMove--;
        }
    }

    if (warped) {
        Warp(warp);
    } else {
        m_currentRoom->Update(m_currentRoom->getX()-dx,m_currentRoom->getY()-dy);
        m_PetitPoint.Move(dx, dy);
    }

}

void LevelState::Warp(const std::string& p_warp)
{
    const std::string& nextMap = m_loads[p_warp];
    m_currentRoom = &m_maps[nextMap];

    const LMap::Zone& zone = m_currentRoom->getLoad(p_warp);
    int middle_x = zone.m_x + zone.m_w/2;
    int middle_y = zone.m_y + zone.m_h/2;

    m_currentRoom->Update(m_pWindow->getWidth()/2 - PetitPoint::IMAGE_SIZE/2 - middle_x,
    m_pWindow->getHeight()/2 - PetitPoint::IMAGE_SIZE/2 - middle_y);
    m_PetitPoint.Warp(m_currentRoom->getName(),
                      zone.m_x + zone.m_w / 2 - PetitPoint::IMAGE_SIZE/2,
                      zone.m_y + zone.m_h / 2 - PetitPoint::IMAGE_SIZE/2);

}

bool initEnemies(const RessourcesRepo&);

}
