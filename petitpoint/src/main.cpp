#define SDL_MAIN_HANDLED

#include "GameMachine.h"

int main( int argc, char* args[] )
{
    pp::GameMachine gameMachine;

	bool success = true;

	if (success)
	{
        success = gameMachine.Init();
	}

	if (success)
	{
        gameMachine.Loop();
	}

    return 0;
}
