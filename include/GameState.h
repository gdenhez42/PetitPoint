#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "LWindow.h"
#include <SDL2/SDL.h>

/*********************************************
State DP so our game can go to different transitions
(title screen, level, dungeon...)

*********************************************/
class GameState
{
    public:
        GameState();
        virtual ~GameState();

        virtual bool Init(const LWindow& p_pWindow)=0;
        virtual GameState* Update(const SDL_Event& e)=0;
        virtual void Render()=0;

    private:
};

#endif // GAMESTATE_H
