#ifndef PERSONAGE_H
#define PERSONAGE_H

#include "Utilities.h"
#include <map>
#include <string>

namespace pp {

    namespace tiled {
        class TileSet;
        class Tile;
    };
    class LAnimation;
    class LMap;
    class LTexture;
    class LevelState;
    class RessourcesRepo;

    class Personage
    {
    public:

        Personage(int, int, const std::string&, int hp);
        virtual ~Personage();

        int getX() const {return m_x;}
        int getY() const {return m_y;}
        int getWidth() const {return m_width;}
        int getHeight() const {return m_height;}
        const std::string& getRoom() const {return m_room;}
        int getHP() const {return m_hp;}
        int getMaxHP() const {return m_maxHp;}
		Rectangle getGroundHb() const;
        void SetPos(int x, int y) { m_x = x; m_y = y; }
        void Move(int dx, int dy) { m_x += dx; m_y += dy; }
        void Warp(const std::string& room, int x, int y) { m_room = room; m_x = x; m_y = y; }


        bool Init(const RessourcesRepo& p_RessourceRepo,
                  const std::string& p_tilesetName);
        void Render(int p_offsetX, int p_offsetY);
        void Update();

        void GiveDamage(int damage);
        void Heal(int hp);

    protected:

        // Position du personnage
        // relatif a la position de la map dans laquelle il est
        std::string m_room;
        int m_x;
        int m_y;
        int m_width;
        int m_height;

        // Les points de vie du personnage
        int m_hp;
        int m_maxHp;
        int m_invinsibleFrames;
        int m_maxInvinsibleFrames;

        // La hitbox pour les collisions contre les murs
        Rectangle m_groundHb;

        // L'animation a afficher pour ce personnage
        std::map<std::string, LAnimation> m_animations;
        LAnimation* m_currentAnimation;

        void InitAnimation(const pp::tiled::TileSet& p_tileSet,
                           const pp::tiled::Tile& p_tile,
                           const pp::LTexture& p_image);
    };

}

#endif // PERSONAGE_H
