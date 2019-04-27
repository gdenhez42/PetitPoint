#ifndef FAKELEVELSTATE_H
#define FAKELEVELSTATE_H

#include "Utilities.h"
#include "PetitPoint.h"
#include "Tiled.h"
#include "Enemy.h"
#include "GameState.h"
#include <vector>

#ifdef WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

class LWindow;
class RessourcesRepo;

namespace pp {

    class LMap;
    class Enemy;

    class LevelState : public GameState
    {
    public:
        LevelState();
        ~LevelState();

        bool Init(const LWindow& p_pWindow, const RessourcesRepo& p_ressourceRepo);
        GameState* Update(const SDL_Event& e, const Uint8*);
        void Render();
        const LMap* getCurrentRoom() const {return m_currentRoom;}
        LMap* getCurrentRoom() {return m_currentRoom;}
        bool CheckCollisions(const Rectangle&) const;
        void Warp(const std::string&);

    private:

        bool initCharacters(const RessourcesRepo&);

        std::map<std::string, LMap> m_maps;
        std::map<std::string, std::string> m_loads;
        std::vector<Enemy> m_enemies;
        LMap* m_currentRoom;
        PetitPoint m_PetitPoint;

        const LWindow* m_pWindow;
    };

}

#endif // FAKELEVELSTATE_H
