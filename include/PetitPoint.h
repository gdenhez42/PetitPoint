#ifndef PETITPOINT_H
#define PETITPOINT_H

#include "LTexture.h"
#include "command.h"

class RessourcesRepo;

class PetitPoint
{
    public:
        PetitPoint();
        virtual ~PetitPoint();

        int getX() {return m_x;}
        int getY() {return m_y;}
        int getWidth() {return m_w;}
        int getHeight() {return m_h;}

        void Init(const RessourcesRepo& resources, int x, int y);
        void Update(Command::Command command);
        void Render();

    private:

        // dimentions de petit point
        int m_x;
        int m_y;
        int m_offsetX;
        int m_offsetY;
        int m_w;
        int m_h;

        // Actions de petit point
        int m_frame;
        int m_direction;
        bool m_moved;

        // Images and clips pour le rendering de petit point
        const LTexture* m_ptexture;
        SDL_Rect m_frontIdle;
        SDL_Rect m_frontWalk[2];
        SDL_Rect m_backIdle;
        SDL_Rect m_backWalk[2];
        SDL_Rect m_leftIdle;
        SDL_Rect m_leftWalk[2];
        SDL_Rect m_rightIdle;
        SDL_Rect m_rightWalk[2];
};

#endif // PETITPOINT_H
