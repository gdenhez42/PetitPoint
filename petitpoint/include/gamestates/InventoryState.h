#ifndef INVENTORYSTATE_H
#define INVENTORYSTATE_H

#include "GameState.h"

#ifdef WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

namespace pp {

    class RessourcesRepo;
    class LWindow;

    class InventoryState : public GameState {
    public:
        InventoryState(GameState*);
        ~InventoryState();
        bool Init(const LWindow& p_pWindow, const RessourcesRepo& p_ressourceRepo);
        std::tuple<bool,GameState*> Update(const Commands& command);
        void Render();

    private:
        GameState* m_previous;
    };
}

#endif // INVENTORYSTATE_H
