#include "Enemy.h"
#include "RessourcesRepo.h"
#include "Tiled.h"

namespace {
    const int IMAGE_SIZE = 64;
    const std::string PIKA_TS = "Assassin";
}

namespace pp {

Enemy::Enemy(int x, int y, const std::string& room)
: Personage(HitBox(0,0,64,64), x,y, room)
{
    //ctor
}

Enemy::~Enemy()
{
    //dtor
}

void Enemy::Init(const RessourcesRepo& resources)
{
    // Init the animations
    m_tileset = &resources.getTileSet(PIKA_TS);
    m_frontIdle = m_tileset->getAnimation(0);

    m_currentAnimation = &m_frontIdle;

}

}
