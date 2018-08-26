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
        const LAnimation* m_frontIdle;
        const LAnimation* m_frontWalk;
        const LAnimation* m_backIdle;
        const LAnimation* m_backWalk;
        const LAnimation* m_leftIdle;
        const LAnimation* m_leftWalk;
        const LAnimation* m_rightIdle;
        const LAnimation* m_rightWalk;

};

}

#endif // PETITPOINT_H
