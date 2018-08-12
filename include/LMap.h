#ifndef LMAP_H
#define LMAP_H

#include <string>
#include <SDL2/SDL.h>
#include <map>

class LWindow;
class RessourcesRepo;
class LTexture;

namespace pp
{

class LMap
{
    public:

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

        LMap();
        virtual ~LMap();

        // Utilities
        int getX() const {return m_x;}
        int getY() const {return m_y;}
        const std::string& getName() const {return m_name;}
        std::vector<std::string> getWarps() const;
        bool isBlocked(int x, int y) const;
        bool isWarp(int x, int y, std::string& p_rWarp) const;
        bool inWarp(int x, int y, int w, int h) const;

        // Rendering loop
        bool Init(const LWindow& p_window,
                  const RessourcesRepo& p_ressourceRepo,
                  const std::string& p_mapName,
                  const std::string& p_name);
        void Render() const;
        void Update(int x, int y);
        void Update(const std::string&);

    private:

        // No copy plz
        LMap(const LMap&);
        const LMap& operator=(LMap);

        std::string m_name;
        int m_width;
        int m_heigth;
        int m_tilewidth;
        int m_tileheight;
        std::map<std::string, Warp> m_warps;

        const LWindow* m_pWindow;
        const LTexture* m_background;
        Tile** m_tiles;
        int m_x;
        int m_y;
};

}

#endif // LMAP_H
