#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Classes.h"
#include "Player.h"


int main(int argc,char* args[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
		return -1;
	}
	if (TTF_Init() < 0) 
	{
		std::cout << " Failed to initialize SDL_ttf : " << SDL_GetError() << std::endl;
		return -1;
	}
	GameClass game;
	game.mainMenu();
	SDL_Quit();
	TTF_Quit();
	return 0;
}