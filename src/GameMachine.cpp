#include "GameMachine.h"
#include "TitleScreenState.h"
#include "FakeLevelState.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

namespace {
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
}

GameMachine::GameMachine()
: m_currentState(nullptr),
  m_Window(SCREEN_WIDTH, SCREEN_HEIGHT),
  m_ressourceRepo()
{
    //ctor
}

GameMachine::~GameMachine()
{
    //dtor
    if (m_currentState != nullptr)
    {
        delete m_currentState;
    }

    //Quit SDL subsystems
    SDL_Quit();
}

bool GameMachine::Init()
{
    //Initialization flag
    bool success = true;
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }

    //Set texture filtering to linear
    if (success)
    {
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }
    }

    if (success)
    {
        //Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if(!(IMG_Init(imgFlags) & imgFlags))
        {
            printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
            success = false;
        }
    }

    //Initialize SDL_ttf
    if (success)
    {
        if( TTF_Init() == -1 )
        {
            printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
            success = false;
        }
    }

    if (success)
    {
        success = m_Window.Load();
    }

    if (success)
    {
        success = m_ressourceRepo.Load(m_Window);
    }

    return success;
}

void GameMachine::Loop()
{
    m_currentState = new TitleScreenState();
    // quit event
    bool quit = !m_currentState->Init(m_Window, m_ressourceRepo);
    // event_handler
    SDL_Event e;

    while (!quit)
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
        }

        if (!quit)
        {
            //Clear screen
            m_Window.Clear();

            GameState* newState = m_currentState->Update(e);
            if (newState != m_currentState)
            {
                quit = !newState->Init(m_Window, m_ressourceRepo);
                m_currentState = newState;
            }
        }

        if (!quit)
        {
            m_currentState->Render();
            //Update screen
            m_Window.Refresh();
        }

    }

}
