#include "LevelState.h"
#include "LMap.h"

namespace {
    const std::string PERSONNAGE = "Personnages";
    const std::string PETITPOINT = "PetitLore";
    const std::string ASSASSIN = "Assassin";

    const std::string MANOIR_WC = "manoir_WC";
    const std::string MANOIR_BIBLI = "manoir_bibli";
    const std::string MANOIR_CORRIDOR = "manoir_corridor";
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
        const tiled::TileMap& manoirWC = p_ressourceRepo.getMap(MANOIR_WC);
        const tiled::TileMap& manoir_bibli = p_ressourceRepo.getMap(MANOIR_BIBLI);
        const tiled::TileMap& manoir_corridor = p_ressourceRepo.getMap(MANOIR_CORRIDOR);

        success = m_maps[MANOIR_WC].Init(p_ressourceRepo, p_pWindow, MANOIR_WC, manoirWC);
        success &= m_maps[MANOIR_BIBLI].Init(p_ressourceRepo, p_pWindow, MANOIR_BIBLI, manoir_bibli);
        success &= m_maps[MANOIR_CORRIDOR].Init(p_ressourceRepo, p_pWindow, MANOIR_CORRIDOR, manoir_corridor);

        if (success) {
            success = initCharacters(p_ressourceRepo);
        }
        if (success) {
            m_currentRoom = &m_maps[m_PetitPoint.getRoom()];

            // Make sure petitpoint is in the middle of the screen
            m_currentRoom->Update(m_pWindow->getWidth()/2 - m_PetitPoint.getX() - m_PetitPoint.getWidth()/2,
                                  m_pWindow->getHeight()/2 - m_PetitPoint.getY() - m_PetitPoint.getHeight()/2);

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
        m_currentRoom->Update(m_pWindow->getWidth()/2 - m_PetitPoint.getX() - m_PetitPoint.getWidth(),
                              m_pWindow->getHeight()/2 - m_PetitPoint.getY() - m_PetitPoint.getHeight());

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

		Rectangle hb = m_PetitPoint.getGroundHb();
		hb.m_x += m_currentRoom->getX();
		hb.m_y += m_currentRoom->getY();
		m_pWindow->RenderRec(hb);
    }

    void LevelState::Warp(const std::string& p_warp)
    {
        const std::string& nextMap = m_loads[p_warp];
        m_currentRoom = &m_maps[nextMap];

        const LMap::Zone& zone = m_currentRoom->getLoad(p_warp);
        int middle_x = zone.m_rec.m_x + zone.m_rec.m_w/2;
        int middle_y = zone.m_rec.m_y + zone.m_rec.m_h/2;

        m_currentRoom->Update(m_pWindow->getWidth()/2 - middle_x,
                              m_pWindow->getHeight()/2 - middle_y);
        m_PetitPoint.Warp(m_currentRoom->getName(),
                          middle_x - m_PetitPoint.getWidth(),
                          middle_y - m_PetitPoint.getHeight());

    }

    /********************************************************************
     Check for collisions with enemies in the room
    ********************************************************************/
    bool LevelState::CheckCollisions(const Rectangle& p_Rec) const
    {
        for (const Enemy& enemy : m_enemies) {

            Rectangle ex = enemy.getGroundHb();

            if (enemy.getRoom() == m_currentRoom->getName() &&
                AreColliding(ex, p_Rec)) {
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
        bool success = true;
        bool foundPetitPoint = false;

        // Find the positions of the characters
        std::map<std::string, LMap>::const_iterator it1 = m_maps.begin();
        std::map<std::string, LMap>::const_iterator itend1 = m_maps.end();
        for (; it1 != itend1; ++it1) {
            const std::string& roomName = it1->first;
            const tiled::TileMap& room = p_ressourceRepo.getMap(it1->second.getName());
            const tiled::ObjectGroup* pNode = nullptr;
            std::map<std::string, tiled::ObjectGroup>::const_iterator objGr = room.m_objects.find(PERSONNAGE);
            if (objGr != room.m_objects.end()) {

                const std::vector<tiled::Object>& objects = objGr->second.m_objects;
                std::vector<tiled::Object>::const_iterator itend2 = objects.end();
                std::vector<tiled::Object>::const_iterator it2 = objects.begin();
                for (; it2 != itend2; ++it2) {
                    int val = it2->m_gid;
                    if (val > 0) {
                        const tiled::TilesetRef& tsr = room.FindTilesetRef(val);
                        const tiled::TileSet& ts = p_ressourceRepo.getTileSet(RessourcesRepo::getRessourceName(tsr.m_source));
                        if (ts.m_name == PETITPOINT) {
                            foundPetitPoint = true;
                            m_PetitPoint.Warp(roomName, it2->m_x, it2->m_y - it2->m_h);
                        }
                        else if (ts.m_name == ASSASSIN) {
                            m_enemies.push_back(Enemy(it2->m_x, it2->m_y - it2->m_h, roomName));
                        }

                    }

                }
            }
        }

        success = foundPetitPoint;

        // Call init for every characters found
        if (success) {
            success = m_PetitPoint.Init(p_ressourceRepo);
            for (Enemy& enemy : m_enemies) {
                success &= enemy.Init(p_ressourceRepo);
            }
        }

        return success;
    }

}
