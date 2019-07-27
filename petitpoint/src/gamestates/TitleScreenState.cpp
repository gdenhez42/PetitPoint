#include "TitleScreenState.h"
#include "LevelState.h"
#include "RessourcesRepo.h"

namespace pp {
    TitleScreenState::TitleScreenState()
        : m_ww(),
          m_wh(),
          m_pFont(NULL),
          m_pTextTexture(nullptr),
          m_pWindow(nullptr),
          m_pRessourceRepo(nullptr)
    {
        //ctor
    }

    TitleScreenState::~TitleScreenState()
    {
        //dtor
    }

    bool TitleScreenState::Init(const LWindow& p_Window, const RessourcesRepo& p_ressourceRepo)
    {
        //Loading success flag
        bool success = true;
        m_ww = p_Window.getWidth();
        m_wh = p_Window.getHeight();

        m_pTextTexture = p_ressourceRepo.getTitleText();
        m_pWindow = &p_Window;
        m_pRessourceRepo = &p_ressourceRepo;

        return success;
    }
    std::tuple<bool, GameState*> TitleScreenState::Update(const Commands& p_command)
    {
        switch(p_command.GetEvent()) {
        case Commands::PP_EVENT_SELECT: {
            GameState* newState = new pp::LevelState();
            bool result = newState->Init(*m_pWindow, *m_pRessourceRepo);
            delete this;
            return std::make_tuple(result, newState);
        }
        default:
            return std::make_tuple(true, this);
        }
    }
    void TitleScreenState::Render()
    {
        //Render current frame
        m_pTextTexture->render((m_ww - m_pTextTexture->getWidth() ) / 2, ( m_wh - m_pTextTexture->getHeight() ) / 2 );
    }
}
