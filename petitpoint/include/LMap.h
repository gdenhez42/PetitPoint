#ifndef LMAP_H
#define LMAP_H

#include "Tiled.h"
#include "Utilities.h"
#include <string>
#include <vector>

#ifdef WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

class LWindow;
class LTexture;

namespace pp
{
    class RessourcesRepo;

    class LMap
    {
    public:

        struct Tile {
            SDL_Rect m_rect;
            bool m_blocked;
            const LTexture* m_texture;
        Tile() : m_rect(), m_blocked(true), m_texture(nullptr) {}
        };

        LMap();
        virtual ~LMap();

        // Utilities
        int getX() const {return m_x;}
        int getY() const {return m_y;}
        const std::string& getName() const {return m_name;}
        bool isBlocked(const Rectangle&) const;
        bool getWarp(const Rectangle&, WarpZone& p_rWarp) const;

        // Rendering loop
        bool Init(const RessourcesRepo& p_ressourceRepo,
                  const LWindow& p_window,
                  const std::string& p_name,
                  const tiled::TileMap& tilemap);
        void Render() const;
        void Update(int x, int y);

    private:

        // No copy plz
        LMap(const LMap&);
        const LMap& operator=(LMap);

        std::string m_name;
        int m_width;
        int m_heigth;
        int m_tilewidth;
        int m_tileheight;
        std::vector<WarpZone> m_warps;

        const LWindow* m_pWindow;
        Tile** m_tiles;

        // Position de la map, relatif au coin superieur gauche de l'ecran
        int m_x;
        int m_y;
    };

}

#endif // LMAP_H
