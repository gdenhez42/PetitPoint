#ifndef LMAP_H
#define LMAP_H

#include <string>
#include <SDL2/SDL.h>

class LWindow;
class RessourcesRepo;
class LTexture;

namespace pp
{

class LMap
{
    public:
        LMap();
        virtual ~LMap();

        // Utilities
        int getX() const {return m_x;}
        int getY() const {return m_y;}
        bool isBlocked(int x, int y) const;
        bool isWarp(int x, int y) const;

        // Rendering loop
        bool Init(const LWindow& p_window, const RessourcesRepo& p_ressourceRepo, const std::string& p_mapName);
        void Render() const;
        void Update(int x, int y);

    private:

        struct Tile {
            SDL_Rect m_rect;
            bool m_blocked;
        };

        struct Warp {
            int m_x;
            int m_y;
            int m_w;
            int m_h;
            std::string m_name;
        };

        std::string m_name;
        int m_width;
        int m_heigth;
        int m_tilewidth;
        int m_tileheight;

        // To render the tiled map
        const LWindow* m_pWindow;
        const LTexture* m_background;
        Tile** m_tiles;
        mutable int m_x;
        mutable int m_y;
};

}

#endif // LMAP_H
