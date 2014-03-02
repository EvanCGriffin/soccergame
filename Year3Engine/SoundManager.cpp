#include "SoundManager.h"

#include <SDL\SDL.h>
#include <SDL\SDL_audio.h>
#include <iostream>

SoundManager::SoundManager(void)
{
	//setup SDL_mixer
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16; // 16-bit 
	int audio_channels = 2; // stereo
	int audio_buffers = 4096; // memory chunk size

	SDL_Init(SDL_INIT_AUDIO);

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
	{
		std::cout << "Unable to open audio" << std::endl;
		SDL_Quit();
	}
}


SoundManager::~SoundManager(void)
{
	std::map<std::string, Sound*>::iterator soundIter;
	std::map<std::string, Music*>::iterator musicIter;

	for(soundIter = sounds.begin(); soundIter != sounds.end(); soundIter++)
		delete soundIter->second;

	for(musicIter = musics.begin(); musicIter != musics.end(); musicIter++)
		delete musicIter->second;

	Mix_CloseAudio();
}

bool SoundManager::AddSound(std::string filename, std::string soundName)
{
	Sound* sound = new Sound();

	if(sound->Load(filename))
	{
		sounds[soundName] = sound; // Store the sound in the map
		return true;
	}

	return false;
}

bool SoundManager::AddMusic(std::string filename, std::string musicName)
{
	Music* music = new Music();

	if(music->Load(filename))
	{
		musics[musicName] = music; // Store the music in the map
		return true;
	}

	return false;
}

void SoundManager::PlaySound(std::string soundName, int loops)
{
	if(sounds.find(soundName) != sounds.end()) // Make sure it exists
		sounds[soundName]->Play(loops);
}

void SoundManager::PlayMusic(std::string musicName, int loops)
{
	if(musics.find(musicName) != musics.end()) // Make sure it exists
		musics[musicName]->Play(loops);
}

// Volume must be between 0 - 128
void SoundManager::setSoundVolume(int volume)
{
	if(volume > 128)
		volume = 128;

	if(volume < 0)
		volume = 0;

	Mix_Volume(-1, volume);

	soundVolume = volume;
}

// Volume must be between 0 - 128
void SoundManager::setMusicVolume(int volume)
{
	if(volume > 128)
		volume = 128;

	if(volume < 0)
		volume = 0;

	Mix_VolumeMusic(volume);

	musicVolume = volume;
}

int SoundManager::getSoundVolume()
{
	return soundVolume;
}

int SoundManager::getMusicVolume()
{
	return musicVolume;
}