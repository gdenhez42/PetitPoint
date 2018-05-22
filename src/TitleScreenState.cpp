#include "TitleScreenState.h"
#include "FakeLevelState.h"


TitleScreenState::TitleScreenState()
: m_ww(),
  m_wh(),
  m_pFont(NULL),
  m_TextTexture()
{
    //ctor
}

TitleScreenState::~TitleScreenState()
{
    //dtor
    if (m_pFont != NULL)
    {
        TTF_CloseFont(m_pFont);
        m_pFont = NULL;
    }
}

bool TitleScreenState::Init(const LWindow& p_pWindow)
{
    //Loading success flag
    bool success = true;
    m_ww = p_pWindow.getWidth();
    m_wh = p_pWindow.getHeight();

    //Open the font
    if (success)
    {
        m_pFont = TTF_OpenFont( "Fonts/lazy.ttf", 28 );
        if( m_pFont == NULL )
        {
            printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
            success = false;
        }
    }

    if (success)
    {
        //Render text
        SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
        if(!m_TextTexture.loadFromRenderedText( p_pWindow, m_pFont, "TBR", textColor ))
        {
            printf( "Failed to render text texture!\n" );
            success = false;
        }
    }

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
                GameState* newState = new FakeLevelState();
                delete this;
                return newState;
        }
    }

    return this;
}
void TitleScreenState::Render()
{
    //Render current frame
    m_TextTexture.render((m_ww - m_TextTexture.getWidth() ) / 2, ( m_wh - m_TextTexture.getHeight() ) / 2 );
}
