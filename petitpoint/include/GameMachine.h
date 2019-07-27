#ifndef GAMEMACHINE_H
#define GAMEMACHINE_H

#include "LWindow.h"
#include "RessourcesRepo.h"
#include "Commands.h"

namespace pp {

    class GameState;

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
        LWindow m_Window;
        RessourcesRepo m_ressourceRepo;
        Commands m_command;
    };

}

#endif // GAMEMACHINE_H

