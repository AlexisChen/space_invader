#include "game.h"



int APIENTRY WinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	LPSTR commandLine,
	int commandShow)
{
	Game myGame;
	myGame.Initialize();
	myGame.RunLoop();
	myGame.ShutDown();
	return 0;
}



