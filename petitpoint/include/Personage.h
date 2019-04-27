#ifndef PERSONAGE_H
#define PERSONAGE_H

#include "Utilities.h"
#include <string>

class RessourcesRepo;

namespace pp {

    class LAnimation;
    class TileSet;
    class LMap;
    class LevelState;

    class Personage
    {
    public:

        Personage(const Rectangle&, int, int, const std::string&);
        virtual ~Personage();

        int getX() const {return m_x;}
        int getY() const {return m_y;}
        const std::string& getRoom() const {return m_room;}
		Rectangle getGroundHb() const;

        void Render(const LevelState& p_LevelState);
        void SetPos(int x, int y) { m_x = x; m_y = y; }
        void Move(int dx, int dy) { m_x += dx; m_y += dy; }
        void Warp(const std::string& room, int x, int y) { m_room = room; m_x = x; m_y = y; }

    protected:

        // Position du personnage, relatif à la position de la map dans laquelle il est
        std::string m_room;
        int m_x;
        int m_y;

        // La hitbox pour les collisions contre les murs
        Rectangle m_groundHb;

        // L'animation a afficher pour ce personnage
        const TileSet* m_tileset;
        LAnimation* m_currentAnimation;

    };

}

#endif // PERSONAGE_H
