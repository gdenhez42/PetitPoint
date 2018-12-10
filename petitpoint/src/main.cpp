#include "GameMachine.h"

int main( int argc, char* args[] )
{
    GameMachine gameMachine;

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
