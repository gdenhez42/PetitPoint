#ifndef TILE_H
#define TILE_H

class LWindow;
class RessourcesRepo;
class SDL_Renderer;

class Tile
{
    public:
        static const int SIZE;

        Tile();
        virtual ~Tile();

        enum Kind {NONE, FLOOR, WALL, WINDOW, DOOR};

        bool Init(const LWindow& window, const RessourcesRepo& resources, Kind tileKind);
        void Render(int x, int y);

        bool IsBlocked() const { return m_isBlocked; }

    protected:
        bool m_isBlocked;
        bool m_isWarp;
        Kind m_kind;
        SDL_Renderer* m_pRenderer;
};

#endif // TILE_H
