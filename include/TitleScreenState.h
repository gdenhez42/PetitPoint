#ifndef TITLESCREENSTATE_H
#define TITLESCREENSTATE_H

#include "GameState.h"
#include "LTexture.h"
#include <SDL2/SDL_ttf.h>

class TitleScreenState : public GameState
{
    public:
        TitleScreenState();
        virtual ~TitleScreenState();

        bool Init(const LWindow& p_pWindow, const RessourcesRepo& p_ressourceRepo);
        GameState* Update(const SDL_Event& e);
        void Render();

    private:
        int m_ww;
        int m_wh;
        const LFont *m_pFont;
        const LTexture *m_pTextTexture;
};

#endif // TITLESCREENSTATE_H
