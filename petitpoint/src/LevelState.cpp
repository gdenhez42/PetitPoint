#include "LevelState.h"
#include "LMap.h"

namespace {
    const std::string PERSONNAGE = "Personnages";
    const std::string PETITPOINT = "Lore";
    const std::string ASSASSIN = "Assassin";
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

    /*********************************************************************
     Update elements in the level
    ********************************************************************/
    GameState* LevelState::Update(const SDL_Event& e, const Uint8* keyboardState)
    {
        // Update petitpoint
        m_PetitPoint.Update(*this, keyboardState);

        // Make sure petitpoint is in the middle of the screen
        m_currentRoom->Update(m_pWindow->getWidth()/2 - m_PetitPoint.getX() - PetitPoint::IMAGE_SIZE/2,
                              m_pWindow->getHeight()/2 - m_PetitPoint.getY() - PetitPoint::IMAGE_SIZE/2);

        return this;
    }

    /*********************************************************************
     Render element in this level
    *******************************************************************/
    void LevelState::Render()
    {
        m_currentRoom->Render();
        for(Enemy& enemy : m_enemies) {
            enemy.Render(*this);
        }
        m_PetitPoint.Render(*this);
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

    /********************************************************************
     Check for collisions with enemies in the room
    ********************************************************************/
    bool LevelState::CheckCollisions(int x, int y, int w, int h) const
    {
        for (const Enemy& enemy : m_enemies) {

            int ex = enemy.hitBoxX();
            int ew = enemy.hitBoxW();
            int ey = enemy.hitBoxY();
            int eh = enemy.hitBoxH();

            if (enemy.getRoom() == m_currentRoom->getName() &&
                !(x + w < ex || ex + ew < x) &&
                !(y + h < ey || ey + eh < y)) {
                return true;
            }
        }
        return false;
    }

    /*********************************************************************
     Init characters in the level
    ********************************************************************/
    bool LevelState::initCharacters(const RessourcesRepo& p_ressourceRepo)
    {
        bool foundPetitPoint = false;

        // Find the positions of the characters
        std::map<std::string, LMap>::const_iterator it = m_maps.begin();
        std::map<std::string, LMap>::const_iterator itend = m_maps.end();
        for (; it != itend; ++it) {
            const TileMap& room = p_ressourceRepo.getMap(it->second.getName());
            const TileMap::ObjectGroup* pNode = nullptr;
            bool havePerso = room.FindObjectGroup(PERSONNAGE, &pNode);
            if (havePerso) {
                const std::vector<TileMap::Object>& objects = pNode->m_objects;
                std::vector<TileMap::Object>::const_iterator itend = objects.end();
                for (std::vector<TileMap::Object>::const_iterator it = objects.begin(); it != itend; ++it) {
                    int val = it->m_gid;
                    if (val > 0) {
                        const TileMap::TilesetNode& ts = room.FindTileset(val);
                        if (ts.m_tileSet.getName() == PETITPOINT) {
                            foundPetitPoint = true;
                            m_PetitPoint.Warp(room.GetName(), it->m_x, it->m_y - it->m_h);
                        }
                        else if (ts.m_tileSet.getName() == ASSASSIN) {
                            m_enemies.push_back(Enemy(it->m_x, it->m_y - it->m_h, room.GetName()));
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
