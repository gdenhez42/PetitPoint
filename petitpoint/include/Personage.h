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
        int hitBoxX() const {return m_x + m_groundHb.m_x;}
        int hitBoxY() const {return m_y + m_groundHb.m_y;}
        int hitBoxW() const {return m_groundHb.m_w;}
        int hitBoxH() const {return m_groundHb.m_h;}
        const std::string& getRoom() const {return m_room;}
        const Rectangle& getGroundHb() const {return m_groundHb;}

        void Render(const LevelState& p_LevelState);
        void SetPos(int x, int y) { m_x = x; m_y = y; }
        void Move(int dx, int dy) { m_x += dx; m_y += dy; }
        virtual void Move(LevelState&, int dx, int dy);
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
