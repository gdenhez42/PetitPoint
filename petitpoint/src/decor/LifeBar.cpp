#include "LifeBar.h"
#include "LWindow.h"
#include "Personage.h"

#ifdef WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

namespace pp {

    bool LifeBar::Init(const Personage* p_pPersonage, const LWindow* p_pWindow)
    {
        m_pPersonage = p_pPersonage;
        m_pRenderer = p_pWindow->getRenderer();
        return true;
    }
    void LifeBar::Render()
    {
        // Draw base lifebar
        SDL_Rect rect = {16,16,206,30};
        SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(m_pRenderer, &rect);

        rect = {17,17,204,28};
        SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(m_pRenderer, &rect);

        rect = {19,19,200,24};
        SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(m_pRenderer, &rect);

        // Draw life in lifebar
        int hp = m_pPersonage->getHP();
        int maxHP = m_pPersonage->getMaxHP();
        int life = hp*200/maxHP;
        rect = {19,19,life,24};
        SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0xFF, 0xAA, 0xFF);
        SDL_RenderFillRect(m_pRenderer, &rect);
    }

}
