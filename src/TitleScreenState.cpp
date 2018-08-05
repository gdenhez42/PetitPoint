#include "TitleScreenState.h"
#include "LevelState.h"


TitleScreenState::TitleScreenState()
: m_ww(),
  m_wh(),
  m_pFont(NULL),
  m_pTextTexture(nullptr)
{
    //ctor
}

TitleScreenState::~TitleScreenState()
{
    //dtor
}

bool TitleScreenState::Init(const LWindow& p_pWindow, const RessourcesRepo& p_ressourceRepo)
{
    //Loading success flag
    bool success = true;
    m_ww = p_pWindow.getWidth();
    m_wh = p_pWindow.getHeight();

    m_pTextTexture = p_ressourceRepo.getTitleText();

    return success;
}
GameState* TitleScreenState::Update(const SDL_Event& e)
{
    if( e.type == SDL_KEYDOWN )
    {
        //Select surfaces based on key press
        switch( e.key.keysym.sym )
        {
            case SDLK_RETURN:
                GameState* newState = new pp::LevelState();
                delete this;
                return newState;
        }
    }

    return this;
}
void TitleScreenState::Render()
{
    //Render current frame
    m_pTextTexture->render((m_ww - m_pTextTexture->getWidth() ) / 2, ( m_wh - m_pTextTexture->getHeight() ) / 2 );
}
