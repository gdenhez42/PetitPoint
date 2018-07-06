#ifndef FAKELEVELSTATE_H
#define FAKELEVELSTATE_H

#include "PetitPoint.h"
#include "Room.h"
#include "GameState.h"
#include <SDL2/SDL.h>

class LWindow;
class RessourcesRepo;

class FakeLevelState : public GameState
{
    public:
        FakeLevelState();
        ~FakeLevelState();

        bool Init(const LWindow& p_pWindow, const RessourcesRepo& p_ressourceRepo);
        GameState* Update(const SDL_Event& e);
        void Render();

    private:
        void MovePetitPointLeft();
        void MovePetitPointRight();
        void MovePetitPointUp();
        void MovePetitPointDown();

        Room m_currentRoom;
        PetitPoint m_PetitPoint;
};

#endif // FAKELEVELSTATE_H
