#include "Sound.h"

#include <iostream>

Sound::Sound()
	:sound(NULL)
{
}


Sound::~Sound(void)
{
	// Release the sound
	if(sound != NULL)
		Mix_FreeChunk(sound);
}

bool Sound::Load(std::string filename)
{
	sound = Mix_LoadWAV_RW(SDL_RWFromFile(filename.c_str(), "rb"), 1);

	if(sound == NULL)
	{
		std::cout << "Failed to load sound " << filename << " " << Mix_GetError() << std::endl;
		return false;
	}

	filename = filename;

	return true;
}

void Sound::Play(int loops)
{
	if(sound == NULL)
		return;

	// Play the sound on the first available channel
	if( Mix_PlayChannel(-1, sound, loops) == -1)
	{
		std::cout << "Failed to play sound " << filename << " " << Mix_GetError() << std::endl;
	}
}
