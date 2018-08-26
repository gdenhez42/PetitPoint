#ifndef ENEMY_H
#define ENEMY_H

#include "Personage.h"
#include "LAnimation.h"

namespace pp {
class Enemy : public Personage
{
    public:
        Enemy(int x, int y, const std::string& room);
        virtual ~Enemy();

        void Init(const RessourcesRepo& resources);

    private:
        const LAnimation* m_frontIdle;

        // Position in the level, relative to the up-left corner of the room it is in
        // N.B. This is not the position where to render the enemy
        std::string m_room;
        int m_tx;
        int m_ty;
};

}

#endif // ENEMY_H
