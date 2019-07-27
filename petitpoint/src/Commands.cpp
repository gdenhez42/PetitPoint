#include "Commands.h"

namespace pp {

    Uint8 Commands::WALK_DOWN = SDL_SCANCODE_DOWN;
    Uint8 Commands::WALK_UP = SDL_SCANCODE_UP;
    Uint8 Commands::WALK_LEFT = SDL_SCANCODE_LEFT;
    Uint8 Commands::WALK_RIGHT = SDL_SCANCODE_RIGHT;

    Commands::Commands()
        : m_keyboardState(0),
          m_event(PP_EVENT_NONE)
    {
    }

    bool Commands::Init()
    {
        m_keyboardState = SDL_GetKeyboardState(NULL);
        return true;
    }

    void Commands::Update()
    {
        m_event = PP_EVENT_NONE;
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0) {
            switch (e.type) {
            case SDL_QUIT:
                m_event = PP_EVENT_QUIT;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                case SDLK_x:
                    m_event = PP_EVENT_SELECT;
                    break;
                case SDLK_z:
                    m_event = PP_EVENT_CANCEL;
                    break;
                }
            }
        }
    }
}
