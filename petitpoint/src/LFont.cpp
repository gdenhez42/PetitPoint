#include "LFont.h"

LFont::LFont()
{
    //ctor
}

LFont::~LFont()
{
    //dtor
    Free();
}

bool LFont::Load(const std::string& p_path, int p_size)
{
    bool success = true;
    m_pFont = TTF_OpenFont( p_path.c_str(), p_size );
    if( m_pFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    return success;
}

void LFont::Free()
{
    if (m_pFont != NULL)
    {
        TTF_CloseFont(m_pFont);
        m_pFont = NULL;
    }
}
