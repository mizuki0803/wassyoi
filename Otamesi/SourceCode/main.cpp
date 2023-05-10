#include "AndosGame.h"

//Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	FrameWork* game = new AndosGame();

	game->Run();

	delete game;

	return 0;
}