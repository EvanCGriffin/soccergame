#include "Music.h"
#include <iostream>

Music::Music()
	:music(NULL)
{
}

Music::~Music(void)
{
	Mix_FreeMusic(music);
}

bool Music::Load(std::string filename)
{
	music = Mix_LoadMUS(filename.c_str());

	if(music == NULL)
	{
		std::cout << "Failed to load music " << filename << " " << Mix_GetError() << std::endl;
		return false;
	}

	filename = filename;

	return true;
}

void Music::Play(int loops)
{
	if(music == NULL)
		return;

	Mix_PlayMusic(music, loops);
}
