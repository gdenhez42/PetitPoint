#ifndef ENEMY_H
#define ENEMY_H

#include "Personage.h"
#include "LAnimation.h"

namespace pp {
    class RessourcesRepo;

    class Enemy : public Personage
    {
    public:
        Enemy(int x, int y, const std::string& room);
        virtual ~Enemy();

        bool Init(const RessourcesRepo& p_Ressources);

    private:
        LAnimation m_frontIdle;
    };

}

#endif // ENEMY_H
