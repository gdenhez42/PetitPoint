#include "Personage.h"
#include "LAnimation.h"
#include "Tiled.h"
#include "LMap.h"

namespace pp {

Personage::Personage(const HitBox& hitbox, int x, int y)
: m_x(x),
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

void Personage::Render(const LMap& currentRoom)
{
    if (m_room == currentRoom.getName()) {
        int x = currentRoom.getX() + m_x;
        int y = currentRoom.getY() + m_y;
        m_currentAnimation->Render(x, y);
    }

}

}
