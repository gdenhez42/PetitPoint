#ifndef LIFEBAR_H
#define LIFEBAR_H

#ifdef WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

namespace pp {
    class Personage;
    class LWindow;

    class LifeBar {
    public:

        bool Init(const Personage*, const LWindow*);
        void Render();
    private:
        SDL_Renderer* m_pRenderer;
        const Personage* m_pPersonage;
    };
}


#endif // LIFEBAR_H

