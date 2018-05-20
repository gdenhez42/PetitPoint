#ifndef FAKEBACKGROUND_H
#define FAKEBACKGROUND_H

#include "LTexture.h"

class FakeBackground
{
    public:
        FakeBackground();
        virtual ~FakeBackground();

        int getX() {return m_x;}
        int getY() {return m_y;}

        bool Load(SDL_Window* p_pWindow, SDL_Renderer* p_pRenderer);
        void Init(int x, int y);
        void Update(int x, int y);
        void Render();

        bool CanMoveThere(int x, int y, int w, int h);

    private:
        // A pointer to the renderer
        SDL_Renderer* m_pRenderer;

        int m_x;
        int m_y;
        int m_wx;
        int m_wy;
        int m_area[40][30];
};

#endif // FAKEBACKGROUND_H
