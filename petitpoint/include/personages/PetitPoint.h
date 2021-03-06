#ifndef PETITPOINT_H
#define PETITPOINT_H

#include "LTexture.h"
#include "Personage.h"
#include "LAnimation.h"
#include "Utilities.h"
#include "Commands.h"

class RessourcesRepo;

namespace pp {

    class LevelState;

    class PetitPoint : public pp::Personage
    {
    public:
        static const int WALK_SPEED;

        PetitPoint();
        virtual ~PetitPoint();

        bool Init(const RessourcesRepo& resources);
        void Update(LevelState&, const Commands& p_command);
        void MovePetitPoint(LevelState&, int, int);

    private:

        int m_direction;
    };

}

#endif // PETITPOINT_H
