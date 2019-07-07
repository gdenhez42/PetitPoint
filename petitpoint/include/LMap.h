#ifndef LMAP_H
#define LMAP_H

#include "Tiled.h"
#include "Utilities.h"
#include <string>
#include <map>

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

        struct Zone {
            Rectangle m_rec;
            std::string m_name;
        Zone(int p_x, int p_y, int p_w, int p_h, const std::string& p_name)
        : m_rec(p_x, p_y, p_w, p_h), m_name(p_name) {}
        };

        LMap();
        virtual ~LMap();

        // Utilities
        int getX() const {return m_x;}
        int getY() const {return m_y;}
        const std::string& getName() const {return m_name;}
        bool isBlocked(const Rectangle&) const;
        bool inWarp(const Rectangle&, std::string&) const;
        std::vector<std::string> getLoads() const;
        const Zone& getLoad(const std::string& p_load) const { return m_loads.at(p_load); }

        // Rendering loop
        bool Init(const RessourcesRepo& p_ressourceRepo,
                  const LWindow& p_window,
                  const std::string& p_name,
                  const tiled::TileMap& tilemap);
        void Render() const;
        void Update(int x, int y);
        void Update(const std::string&);

        // Utilities to make things more user friendly
        int AlignH(const Rectangle&) const;
        bool AlignV(Rectangle&) const;

    private:

        // No copy plz
        LMap(const LMap&);
        const LMap& operator=(LMap);

        std::string m_name;
        int m_width;
        int m_heigth;
        int m_tilewidth;
        int m_tileheight;
        std::map<std::string, Zone> m_warps;
        std::map<std::string, Zone> m_loads;

        const LWindow* m_pWindow;
        Tile** m_tiles;

        // Position de la map, relatif au coin superieur gauche de l'ecran
        int m_x;
        int m_y;
    };

}

#endif // LMAP_H
