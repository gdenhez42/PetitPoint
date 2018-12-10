#ifndef FAKELEVELSTATE_H
#define FAKELEVELSTATE_H

#include "PetitPoint.h"
#include "Tiled.h"
#include "Enemy.h"
#include "GameState.h"
#include <SDL2/SDL.h>
#include <vector>

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

    private:

        enum Dir { LEFT, RIGHT, UP, DOWN};

        bool initCharacters(const RessourcesRepo&);
        void MovePetitPoint(Dir);
        void Warp(const std::string&);

        std::map<std::string, LMap> m_maps;
        std::map<std::string, std::string> m_loads;
        std::vector<Enemy> m_enemies;
        LMap* m_currentRoom;
        PetitPoint m_PetitPoint;

        const LWindow* m_pWindow;
};

}

#endif // FAKELEVELSTATE_H