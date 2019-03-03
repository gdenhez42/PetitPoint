#ifndef PETITPOINT_H
#define PETITPOINT_H

#include "LTexture.h"
#include "Personage.h"
#include "LAnimation.h"
#include "Utilities.h"

class RessourcesRepo;

namespace pp {

    class LevelState;

    class PetitPoint : public pp::Personage
    {
    public:
        static const int WALK_SPEED;
        static const int IMAGE_SIZE;

        PetitPoint();
        virtual ~PetitPoint();

        void Init(const RessourcesRepo& resources);
        void Update(LevelState&, const Uint8*);
        void MovePetitPoint(LevelState&, Dir d);

    private:

        int m_direction;
        LAnimation m_frontIdle;
        LAnimation m_frontWalk;
        LAnimation m_backIdle;
        LAnimation m_backWalk;
        LAnimation m_leftIdle;
        LAnimation m_leftWalk;
        LAnimation m_rightIdle;
        LAnimation m_rightWalk;

    };

}

#endif // PETITPOINT_H
