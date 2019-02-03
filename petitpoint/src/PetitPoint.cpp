#include "PetitPoint.h"
#include "RessourcesRepo.h"
#include "Tiled.h"

namespace {

    enum Directions {
        DOWN,UP,LEFT,RIGHT
    };
    const std::string PETITPOINT_TS = "Lore";

}

namespace pp {

const int PetitPoint::WALK_SPEED = 2;
const int PetitPoint::IMAGE_SIZE = 64;

PetitPoint::PetitPoint()
: Personage(HitBox(12,40,40,22), 0, 0, ""),
  m_direction(DOWN),
  m_frontIdle(),
  m_frontWalk(),
  m_backIdle(),
  m_backWalk(),
  m_leftIdle(),
  m_leftWalk(),
  m_rightIdle(),
  m_rightWalk()
{
}

PetitPoint::~PetitPoint()
{
    //dtor
}

void PetitPoint::Init(const RessourcesRepo& resources)
{
    // Init the animations
    m_tileset = &resources.getTileSet(PETITPOINT_TS);
    m_frontIdle = m_tileset->getAnimation(0);
    m_frontWalk = m_tileset->getAnimation(2);
    m_backIdle = m_tileset->getAnimation(4);
    m_backWalk = m_tileset->getAnimation(6);
    m_rightIdle = m_tileset->getAnimation(8);
    m_rightWalk = m_tileset->getAnimation(10);
    m_leftIdle = m_tileset->getAnimation(12);
    m_leftWalk = m_tileset->getAnimation(14);
    m_currentAnimation = &m_frontIdle;

}
void PetitPoint::Update(Command::Command command)
{
    switch (command)
    {
        case Command::NONE:
            switch(m_direction) {
                case LEFT:
                m_currentAnimation = &m_leftIdle;
                break;
                case RIGHT:
                m_currentAnimation = &m_rightIdle;
                break;
                case UP:
                m_currentAnimation = &m_backIdle;
                break;
                case DOWN:
                m_currentAnimation = &m_frontIdle;
                break;
            }
            break;
        case Command::MOVE_LEFT:
            m_direction = LEFT;
            if (m_currentAnimation != &m_leftWalk) {
                m_currentAnimation = &m_leftWalk;
                m_currentAnimation->Reset();
            }
            break;
        case Command::MOVE_RIGHT:
            m_direction = RIGHT;
            if (m_currentAnimation != &m_rightWalk) {
                m_currentAnimation = &m_rightWalk;
                m_currentAnimation->Reset();
            }
            break;
        case Command::MOVE_UP:
            m_direction = UP;
            if (m_currentAnimation != &m_backWalk) {
                m_currentAnimation = &m_backWalk;
                m_currentAnimation->Reset();
            }
            break;
        case Command::MOVE_DOWN:
            m_direction = DOWN;
            if (m_currentAnimation != &m_frontWalk) {
                m_currentAnimation = &m_frontWalk;
                m_currentAnimation->Reset();
            }
            break;
    }
}

}

