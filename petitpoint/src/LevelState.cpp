#include "LevelState.h"
#include "LMap.h"

namespace {
    const std::string PERSONNAGE = "Personnages";
    const std::string PETITPOINT = "Lore";
}
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
    const TileMap& manoirWC = p_ressourceRepo.getMap("manoir_WC");
    const TileMap& manoir_bibli = p_ressourceRepo.getMap("manoir_bibli");
    const TileMap& manoir_corridor = p_ressourceRepo.getMap("manoir_corridor");

    success = m_maps[manoirWC.GetName()].Init(p_pWindow, manoirWC);
    success &= m_maps[manoir_bibli.GetName()].Init(p_pWindow, manoir_bibli);
    success &= m_maps[manoir_corridor.GetName()].Init(p_pWindow, manoir_corridor);

    if (success) {
        success = initCharacters(p_ressourceRepo);
    }
    if (success) {
        m_currentRoom = &m_maps[m_PetitPoint.getRoom()];

        // Make sure petitpoint is in the middle of the screen
        m_currentRoom->Update(m_pWindow->getWidth()/2 - m_PetitPoint.getX() - PetitPoint::IMAGE_SIZE/2,
                              m_pWindow->getHeight()/2 - m_PetitPoint.getY() - PetitPoint::IMAGE_SIZE/2);

        // Get warp list to move from one room to another
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
    for(Enemy& enemy : m_enemies) {
        enemy.Render(*m_currentRoom);
    }
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

    m_currentRoom->Update(m_pWindow->getWidth()/2 - middle_x,
                          m_pWindow->getHeight()/2 - middle_y);
    m_PetitPoint.Warp(m_currentRoom->getName(),
                      middle_x - PetitPoint::IMAGE_SIZE/2,
                      middle_y - PetitPoint::IMAGE_SIZE/2);

}

/**********************************************************************
Init characters in the level
***********************************************************************/
bool LevelState::initCharacters(const RessourcesRepo& p_ressourceRepo) {

    bool foundPetitPoint = false;

    // Find the positions of the characters
    std::map<std::string, LMap>::const_iterator it = m_maps.begin();
    std::map<std::string, LMap>::const_iterator itend = m_maps.end();
    for (; it != itend; ++it) {
        const TileMap& room = p_ressourceRepo.getMap(it->second.getName());
        const TileMap::LayerNode* pNode = nullptr;
        bool havePerso = room.FindLayerNode(PERSONNAGE, &pNode);
        if (havePerso) {
            int** tiles = pNode->m_tiles;
            for (int i = 0; i < pNode->m_heigth; i++) {
                for (int j = 0; j < pNode->m_width; j++) {
                    int val = tiles[i][j];
                    if (val > 0) {
                        const TileMap::TilesetNode& ts = room.FindTileset(val);
                        if (ts.m_tileSet.getName() == PETITPOINT) {
                            foundPetitPoint = true;
                            m_PetitPoint.Warp(room.GetName(), j*room.GetTilewidth(), i*room.GetTileheight());
                        }
                        else
                        {
                            m_enemies.push_back(Enemy(j*room.GetTilewidth(), i*room.GetTileheight(), room.GetName()));
                        }
                    }
                }
            }
        }
    }

    // Call init for every characters found
    if (foundPetitPoint) {
        m_PetitPoint.Init(p_ressourceRepo);
        for (Enemy& enemy : m_enemies) {
            enemy.Init(p_ressourceRepo);
        }
    }

    return foundPetitPoint;
}

}
