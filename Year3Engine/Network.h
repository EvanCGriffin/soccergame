#pragma once

//include sdl_net
#include <SDL\SDL_net.h>

class Network
{
public:
	//initialize SDL_net
	static void Init();
	//quit SDL_net
	static void Quit();
};

