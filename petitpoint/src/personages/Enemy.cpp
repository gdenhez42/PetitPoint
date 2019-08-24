#include "Enemy.h"
#include "RessourcesRepo.h"
#include "Tiled.h"

namespace {
    const int IMAGE_SIZE = 64;
    const std::string PIKA_TS = "Assassin";
}

namespace pp {

    Enemy::Enemy(int x, int y, const std::string& room)
        : Personage(x,y, room, 30)
    {
        //ctor
    }

    Enemy::~Enemy()
    {
        //dtor
    }

    bool Enemy::Init(const RessourcesRepo& p_Ressources)
    {
        // Init the animations
        bool success = Personage::Init(p_Ressources, PIKA_TS);

        if (success) {
            m_currentAnimation = &m_animations.begin()->second;
        }

        return success;
    }

}
