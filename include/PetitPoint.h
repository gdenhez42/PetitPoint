#ifndef PETITPOINT_H
#define PETITPOINT_H

#include "LTexture.h"

class PetitPoint
{
    public:
        PetitPoint();
        virtual ~PetitPoint();

        int getX() {return m_x;}
        int getY() {return m_y;}
        int getWidth() {return m_w;}
        int getHeight() {return m_h;}

        bool Load(SDL_Renderer* p_pRenderer);
        void Init(int x, int y);
        void Update(bool moved);
        void Render();

    private:
        int m_x;
        int m_y;
        int m_w;
        int m_h;
        int m_frame;
        LTexture m_texture;
        SDL_Rect m_clips[4];
};

#endif // PETITPOINT_H
