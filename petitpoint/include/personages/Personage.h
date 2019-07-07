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

        Personage(int, int, const std::string&);
        virtual ~Personage();

        int getX() const {return m_x;}
        int getY() const {return m_y;}
        int getWidth() const {return m_width;}
        int getHeight() const {return m_height;}
        const std::string& getRoom() const {return m_room;}
		Rectangle getGroundHb() const;
        void SetPos(int x, int y) { m_x = x; m_y = y; }
        void Move(int dx, int dy) { m_x += dx; m_y += dy; }
        void Warp(const std::string& room, int x, int y) { m_room = room; m_x = x; m_y = y; }


        bool Init(const RessourcesRepo& p_RessourceRepo,
                  const std::string& p_tilesetName);
        void Render(const LevelState& p_LevelState);

    protected:

        // Position du personnage
        // relatif a la position de la map dans laquelle il est
        std::string m_room;
        int m_x;
        int m_y;
        int m_width;
        int m_height;


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
