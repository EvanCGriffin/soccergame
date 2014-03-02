#pragma once

#include <string>
#include <SDL\SDL_mixer.h>

// A background song
class Music
{
public:
	Music();
	~Music(void);

	bool Load(std::string filename); // Load the music
	void Play(int loops = 0); // Play the music, with loops, -1 is infinite, 0 is once

private:
	Mix_Music* 	music; // The music data	
	std::string filename; // The filepath to the music file
};

