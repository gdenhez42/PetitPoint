#include "GameMachine.h"
#include "GameState.h"
#include "TitleScreenState.h"
#include "LevelState.h"
#include "RessourcesRepo.h"
#include "Utilities.h"

#ifdef WIN32
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif


namespace {
    const int SCREEN_WIDTH = 960;
    const int SCREEN_HEIGHT = 640;
}

namespace pp {

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
    if (m_currentState != nullptr) {
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
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        Log(std::string("SDL could not initialize! SDL_Error: ") + SDL_GetError());
        success = false;
    }

    //Set texture filtering to linear
    if (success) {
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            Log(std::string("Warning: Linear texture filtering not enabled!"));
        }
    }

    if (success)
    {
        //Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if(!(IMG_Init(imgFlags) & imgFlags))
        {
            Log(std::string("SDL_image could not initialize! SDL_image Error: ") + IMG_GetError());
            success = false;
        }
    }

    //Initialize SDL_ttf
    if (success) {
        if( TTF_Init() == -1 )
        {
            Log(std::string("SDL_ttf could not initialize! SDL_ttf Error: ") + TTF_GetError());
            success = false;
        }
    }

    if (success) {
        success = m_Window.Load();
    }

    if (success) {
        success = m_ressourceRepo.Load(m_Window);
    }

    if (success) {
        success = m_command.Init();
    }

    return success;
}

void GameMachine::Loop()
{
    m_currentState = new TitleScreenState();

    // quit event
    bool quit = !m_currentState->Init(m_Window, m_ressourceRepo);

    while (!quit) {

        m_command.Update();
        quit = m_command.GetEvent() == Commands::PP_EVENT_QUIT;

        if (!quit)
        {
            //Clear screen
            m_Window.Clear();

            std::tuple<bool, GameState*> result = m_currentState->Update(m_command);
            quit = !std::get<0>(result);
            m_currentState = std::get<1>(result);
        }

        if (!quit)
        {
            m_currentState->Render();
            //Update screen
            m_Window.Refresh();
        }

    }

}

}
