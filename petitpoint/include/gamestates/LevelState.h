#ifndef FAKELEVELSTATE_H
#define FAKELEVELSTATE_H

#include "Utilities.h"
#include "PetitPoint.h"
#include "Tiled.h"
#include "Enemy.h"
#include "GameState.h"
#include "LifeBar.h"
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
    class Warp;
    class Enemy;

    class LevelState : public GameState
    {
    public:
        LevelState();
        ~LevelState();

        bool Init(const LWindow& p_pWindow, const RessourcesRepo& p_ressourceRepo);
        std::tuple<bool, GameState*> Update(const Commands& p_command);
        void Render();
        const LMap* getCurrentRoom() const {return m_currentRoom;}
        LMap* getCurrentRoom() {return m_currentRoom;}
        bool CheckCollisions(const Rectangle&) const;
        void Warp(const WarpZone& p_Warp);

    private:

        bool initCharacters(const RessourcesRepo&);

        // Rooms in the level
        std::map<std::string, LMap> m_maps;
        LMap* m_currentRoom;

        // Personage in the level
        PetitPoint m_PetitPoint;
        std::vector<Enemy> m_enemies;

        // Decor element
        LifeBar m_lifeBar;

        // Rendering utilities
        const LWindow* m_pWindow;
        const RessourcesRepo* m_pRessourceRepo;
    };

}

#endif // FAKELEVELSTATE_H
