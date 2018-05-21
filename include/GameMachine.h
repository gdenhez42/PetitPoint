#ifndef GAMEMACHINE_H
#define GAMEMACHINE_H

#include "LWindow.h"
#include "GameState.h"

/***********************************************
The game machine is the component that holds the
game state
***********************************************/
class GameMachine
{
    public:
        GameMachine();
        virtual ~GameMachine();

        bool Init();
        void Loop();

    private:
        GameState* m_currentState;
        LWindow m_Window;
};

#endif // GAMEMACHINE_H
