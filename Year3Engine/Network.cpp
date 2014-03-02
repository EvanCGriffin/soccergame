#include "Network.h"

//initialize SDL_net
void Network::Init()
{
	//initialize sdl_net
	SDLNet_Init();
}

//quit SDL_net
void Network::Quit()
{
	//quit sdl_net
	SDLNet_Quit();
}
