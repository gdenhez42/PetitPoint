#ifndef FAKELEVELSTATE_H
#define FAKELEVELSTATE_H

#include "FakeBackground.h"
#include "PetitPoint.h"
#include "GameState.h"
#include "LWindow.h"
#include <SDL2/SDL.h>

class FakeLevelState : public GameState
{
    public:
        FakeLevelState();
        ~FakeLevelState();

        bool Init(const LWindow& p_pWindow);
        GameState* Update(const SDL_Event& e);
        void Render();

    private:
        FakeBackground m_FakeBackground;
        PetitPoint m_PetitPoint;
};

#endif // FAKELEVELSTATE_H
