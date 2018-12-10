#ifndef GAMEMACHINE_H
#define GAMEMACHINE_H

#include "LWindow.h"
#include "GameState.h"
#include <SDL2/SDL.h>

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

        // to delete
        const LWindow& getWindow() {return m_Window;}

    private:
        GameState* m_currentState;
        const Uint8 * m_keyboardState;
        LWindow m_Window;
        RessourcesRepo m_ressourceRepo;
};

#endif // GAMEMACHINE_H
