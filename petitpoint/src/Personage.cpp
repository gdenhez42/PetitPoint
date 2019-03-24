#include "Personage.h"
#include "LAnimation.h"
#include "Tiled.h"
#include "LMap.h"
#include "LevelState.h"

namespace pp {

    Personage::Personage(const HitBox& hitbox, int x, int y, const std::string& room)
        : m_room(room),
          m_x(x),
          m_y(y),
          m_groundHb(hitbox),
          m_tileset(nullptr),
          m_currentAnimation(nullptr)
    {
        //ctor
    }

    Personage::~Personage()
    {
        //dtor
    }

    void Personage::Render(const LevelState& p_LevelState)
    {
        const LMap* currentRoom = p_LevelState.getCurrentRoom();
        if (m_room == currentRoom->getName()) {
            int x = currentRoom->getX() + m_x;
            int y = currentRoom->getY() + m_y;
            m_currentAnimation->Render(x, y);
        }

    }

    void Personage::Move(LevelState& p_rLevelState, int p_dx, int p_dy)
    {
        
    }
}
