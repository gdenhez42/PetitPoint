#ifndef COMMANDS_H
#define COMMANDS_H

#ifdef WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

namespace pp {
    class Commands {
    public:
        // Events
        enum PP_Event {
            PP_EVENT_NONE,
            PP_EVENT_QUIT,
            PP_EVENT_CANCEL,
            PP_EVENT_SELECT
        };

        // Keyboard State
        static Uint8 WALK_DOWN;
        static Uint8 WALK_UP;
        static Uint8 WALK_LEFT;
        static Uint8 WALK_RIGHT;

        Commands();

        bool Init();
        void Update();

        PP_Event GetEvent() const { return m_event; }
        const Uint8* GetKeyboardState() const { return m_keyboardState; }

    private:
        const Uint8* m_keyboardState;
        PP_Event m_event;
    };
}


#endif // COMMANDS_H
