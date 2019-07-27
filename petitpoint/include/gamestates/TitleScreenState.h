#ifndef TITLESCREENSTATE_H
#define TITLESCREENSTATE_H

#include "GameState.h"
#include "LTexture.h"

namespace pp {

class TitleScreenState : public GameState
{
    public:
        TitleScreenState();
        virtual ~TitleScreenState();

        bool Init(const LWindow& p_window, const RessourcesRepo& p_ressourceRepo);
        std::tuple<bool, GameState*> Update(const Commands& p_command);
        void Render();

    private:
        int m_ww;
        int m_wh;
        const LFont *m_pFont;
        const LTexture *m_pTextTexture;
        const LWindow *m_pWindow;
        const RessourcesRepo *m_pRessourceRepo;
};

}

#endif // TITLESCREENSTATE_H
