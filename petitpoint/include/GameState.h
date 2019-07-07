#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "LWindow.h"
#include "RessourcesRepo.h"
#ifdef WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

namespace pp {

/*********************************************
State DP so our game can go to different transitions
(title screen, level, dungeon...)

*********************************************/
class GameState
{
    public:
        GameState();
        virtual ~GameState();

        virtual bool Init(const LWindow& p_pWindow, const RessourcesRepo& p_ressourceRepo)=0;
        virtual GameState* Update(const SDL_Event& e, const Uint8*)=0;
        virtual void Render()=0;

    private:
};

}

#endif // GAMESTATE_H
