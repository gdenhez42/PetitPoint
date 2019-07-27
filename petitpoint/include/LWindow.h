#ifndef LWINDOW_H
#define LWINDOW_H

#include "Utilities.h"

#ifdef WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

namespace pp {

    class LWindow
    {
    public:
        LWindow(int p_w, int p_h);
        virtual ~LWindow();

        int getWidth() const {return m_w;}
        int getHeight() const {return m_h;}
        SDL_Renderer* getRenderer() const {return m_pRenderer;}


        bool Load();
        void Clear();
        void Refresh();

        // Some debugging utilities
        void RenderRec(const pp::Rectangle&) const;

    private:
        void Free();

        int m_w;
        int m_h;

        // The window we'll be rendering to
        SDL_Window* m_pWindow;

        //The window renderer
        SDL_Renderer* m_pRenderer;
    };

}

#endif // LWINDOW_H
