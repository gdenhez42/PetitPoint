#ifndef ROOM_H
#define ROOM_H

#include <string>

class Tile;
class LWindow;
class RessourcesRepo;

class Room
{
    public:
        Room();
        virtual ~Room();

        int getX() const {return m_x;}
        int getY() const {return m_y;}

        bool Load(const LWindow& window, const RessourcesRepo& resources, const std::string& p_roomPath);
        void Render();
        void Update(int x, int y);
        void Free();
        const Tile& getTile(int x, int y) const;

    private:
        Tile** m_tiles;
        int m_height;
        int m_width;
        int m_x;
        int m_y;
        const LWindow* m_pWindow;
};

#endif // ROOM_H
