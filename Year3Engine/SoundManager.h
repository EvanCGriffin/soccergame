#pragma once

#include <string>
#include <map>

#include "Sound.h"
#include "Music.h"

// Holds a collection of music and sounds
class SoundManager
{
public:
	SoundManager(void);
	~SoundManager(void);

	// Add a sound to the collection with a specified name
	bool AddSound(std::string filename, std::string soundName);
	// Add a music to the collection with a specified name
	bool AddMusic(std::string filename, std::string musicName);

	// Play a sound, loops, -1 is infinite, 0 is once
	void PlaySound(std::string soundName, int loops = 0);
	// Play a song, loops, -1 is infinite, 0 is once
	void PlayMusic(std::string musicName, int loops = 0);

	// Set the volume of soundfx
	void setSoundVolume(int volume);
	// Set the volume of background music
	void setMusicVolume(int volume);
	// Get the volume of the sound
	int getSoundVolume();
	// Get the volume of the background music
	int getMusicVolume();

private:
	std::map<std::string, Sound*> sounds; // map of sounds and their names
	std::map<std::string, Music*> musics; // map of songs and their names
	int soundVolume; // the volume of soundfx
	int musicVolume; // the volume of background music
};

