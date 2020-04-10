#include "LevelState.h"
#include "InventoryState.h"
#include "LMap.h"
#include "RessourcesRepo.h"
#include "LWindow.h"

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
    }

    if (success) {
      m_lifeBar.Init(&m_PetitPoint, m_pWindow);
    }

    return success;
  }

  /*********************************************************************
     Update elements in the level
  ********************************************************************/
  std::tuple<bool,GameState*> LevelState::Update(const Commands& p_command)
  {
    // Check menuing
    switch(p_command.GetEvent()) {
    case Commands::PP_EVENT_SELECT:
      GameState* newState = new InventoryState(this);
      bool result = newState->Init(*m_pWindow, *m_pRessourceRepo);
      return std::make_tuple(result, newState);
    }

    // Update petitpoint
    m_PetitPoint.Update(*this, p_command);

    CheckCollisions();

    // Make sure petitpoint is in the middle of the screen
    m_currentRoom->Update(m_pWindow->getWidth()/2 - m_PetitPoint.getX() - m_PetitPoint.getWidth(),
                          m_pWindow->getHeight()/2 - m_PetitPoint.getY() - m_PetitPoint.getHeight());

    return std::make_tuple(true, this);
  }

  /*********************************************************************
     Render element in this level
  *******************************************************************/
  void LevelState::Render()
  {
    std::string roomName = m_currentRoom->getName();
    int offsetX = m_currentRoom->getX();
    int offsetY = m_currentRoom->getY();

    m_currentRoom->Render();
    for(Enemy& enemy : m_enemies) {
      if (enemy.getRoom() == roomName) {
        enemy.Render(offsetX, offsetY);
      }
    }
    m_PetitPoint.Render(offsetX, offsetY);

    m_lifeBar.Render();
  }

  void LevelState::Warp(const WarpZone& p_warp)
  {
    std::string nextMap = p_warp.m_load.m_room;
    m_currentRoom = &m_maps[nextMap];

    int middle_x = p_warp.m_load.m_rect.m_x + p_warp.m_load.m_rect.m_w/2;
    int middle_y = p_warp.m_load.m_rect.m_y + p_warp.m_load.m_rect.m_h/2;

    m_currentRoom->Update(m_pWindow->getWidth()/2 - middle_x,
                          m_pWindow->getHeight()/2 - middle_y);
    m_PetitPoint.Warp(m_currentRoom->getName(),
                      middle_x - m_PetitPoint.getWidth(),
                      middle_y - m_PetitPoint.getHeight());

  }

  /********************************************************************
     Check for collisions with enemies in the room
  ********************************************************************/
  void LevelState::CheckCollisions()
  {
    Rectangle ppghb = m_PetitPoint.getGroundHb();

    for (const Enemy& enemy : m_enemies) {

      Rectangle ex = enemy.getGroundHb();

      if (enemy.getRoom() == m_currentRoom->getName() &&
          AreColliding(ex, ppghb)) {
        m_PetitPoint.GiveDamage(1);
      }
    }
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
            const tiled::EmbeddedTileset& tsr = room.FindTilesetRef(val);
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
