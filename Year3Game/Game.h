#pragma once

#include <Year3Engine/Engine.h>
#include <string>

class Game
{
public:
	Game(int screenWidth, int screenHeight, int bitsPerPixel, b2Vec2 gravity);
	~Game(void);

	int Intialise(std::string windowTitle);

	void Run(void);

	Engine engine;
};

