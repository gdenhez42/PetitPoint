#ifndef TITLESCREENSTATE_H
#define TITLESCREENSTATE_H

#include "GameState.h"
#include <SDL2/SDL_ttf.h>

class TitleScreenState : public GameState
{
    public:
        TitleScreenState();
        virtual ~TitleScreenState();

        bool Init(const LWindow& p_pWindow);
        GameState* Update(const SDL_Event& e);
        void Render();

    private:
        TTF_Font *m_pFont;
        LTexture m_TextTexture;
};

#endif // TITLESCREENSTATE_H
