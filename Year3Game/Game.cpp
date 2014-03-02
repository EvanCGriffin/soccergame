#include "Game.h"

#include <Year3Engine\GameScreen.h>
#include <iostream>

#include "MainMenuScreen.h"

Game::Game(int screenWidth, int screenHeight, int bitsPerPixel, b2Vec2 gravity)
	: engine(gravity)
{
	engine.ScreenWidth = screenWidth;
	engine.ScreenHeight = screenHeight;
	engine.BitsPerPixel = bitsPerPixel;
}

int Game::Intialise(std::string windowTitle)
{
	if(engine.Init(windowTitle) == -1)
		return -1;

	// Load the mainmenu screen
	engine.AddScreen(new MainMenuScreen());

	return 0;
}

void Game::Run()
{
	engine.Run();
}

Game::~Game(void)
{
}
