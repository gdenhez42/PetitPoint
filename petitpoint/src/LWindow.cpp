#include "LWindow.h"

#include <stdio.h>

#ifdef WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

LWindow::LWindow(int p_w, int p_h)
: m_w(p_w),
  m_h(p_h),
  m_pWindow(NULL),
  m_pRenderer(NULL)
{
    //ctor
}

LWindow::~LWindow()
{
    //dtor
    Free();
}

bool LWindow::Load()
{
    Free();
    //Initialization flag
    bool success = true;

    //Create window
    m_pWindow = SDL_CreateWindow( "Petit point", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_w, m_h, SDL_WINDOW_SHOWN );
    if(m_pWindow == NULL)
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }

    if (success)
    {
        //Create renderer for window
        m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if(m_pRenderer == NULL)
        {
            printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
    }

    return success;
}

void LWindow::Free()
{
    //Destroy window
    if (m_pRenderer != NULL)
    {
        SDL_DestroyRenderer(m_pRenderer);
        m_pRenderer = NULL;
    }
    if (m_pWindow != NULL)
    {
        SDL_DestroyWindow(m_pWindow);
        m_pWindow = NULL;
    }
}

void LWindow::Clear()
{
    SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(m_pRenderer);
}

void LWindow::Refresh()
{
    SDL_RenderPresent(m_pRenderer);
}

void LWindow::RenderRec(const pp::Rectangle& p_Rec) const
{
	SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_Rect rec = { p_Rec.m_x, p_Rec.m_y, p_Rec.m_w, p_Rec.m_h };
	SDL_RenderDrawRect(m_pRenderer, &rec);
}