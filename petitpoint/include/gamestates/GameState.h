#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Commands.h"
#include <tuple>

#ifdef WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

namespace pp {

    class RessourcesRepo;
    class LWindow;

    /*********************************************
State DP so our game can go to different transitions
(title screen, level, dungeon...)

    *********************************************/
    class GameState {

    public:
        GameState();
        virtual ~GameState();

        virtual bool Init(const LWindow& p_pWindow, const RessourcesRepo& p_ressourceRepo)=0;
        virtual std::tuple<bool,GameState*> Update(const Commands& command)=0;
        virtual void Render()=0;

    private:
    };

}

#endif // GAMESTATE_H
