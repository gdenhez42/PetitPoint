#ifndef PERSONAGE_H
#define PERSONAGE_H

#include <string>

class RessourcesRepo;

namespace pp {

class LAnimation;
class TileSet;
class LMap;

class Personage
{
    public:
        // Une hitbox, comme il y en a dans tous les jeux
        struct HitBox {
            int m_x; // position x de la hitbox, relatif a la position du perso
            int m_y; // position y de la hitbox, relatif a la position du perso
            int m_w; // longueur de la hitbox
            int m_h; // hauteur de la hitbox
            HitBox(int x, int y, int w, int h) : m_x(x), m_y(y), m_w(w), m_h(h) {}
        };

        Personage(const HitBox&, int, int);
        virtual ~Personage();

        int getX() {return m_x;}
        int getY() {return m_y;}
        const HitBox& getGroundHb() {return m_groundHb;}

        void Render(const LMap& currentRoom);
        void SetPos(int x, int y) { m_x = x; m_y = y; }
        void Move(int dx, int dy) { m_x += dx; m_y += dy; }
        void Warp(const std::string& room, int x, int y) { m_room = room; m_x = x; m_y = y; }

    protected:

        // Position du personnage, relatif à la position de la map dans laquelle il est
        std::string m_room;
        int m_x;
        int m_y;

        // La hitbox pour les collisions contre les murs
        HitBox m_groundHb;

        // L'animation a afficher pour ce personnage
        const TileSet* m_tileset;
        const LAnimation* m_currentAnimation;

};

}

#endif // PERSONAGE_H
