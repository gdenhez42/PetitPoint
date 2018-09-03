#ifndef PETITPOINT_H
#define PETITPOINT_H

#include "LTexture.h"
#include "command.h"
#include "Personage.h"
#include "LAnimation.h"

class RessourcesRepo;

namespace pp {

class PetitPoint : public pp::Personage
{
    public:
        static const int WALK_SPEED;
        static const int IMAGE_SIZE;

        PetitPoint();
        virtual ~PetitPoint();

        void Init(const RessourcesRepo& resources);
        void Update(Command::Command command);

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
