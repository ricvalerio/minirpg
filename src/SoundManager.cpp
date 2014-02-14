/*
 * SoundManager.cpp
 *
 *  Created on: 16 de Jan de 2014
 *      Author: Ricardo
 */

#include <string>
#include <map>
#include <SDL2/SDL_mixer.h>
#include "SoundManager.h"


std::map<std::string, Mix_Chunk *> SoundManager::sounds;
std::map<std::string, Mix_Music *> SoundManager::musics;


SoundManager::SoundManager() {
	// TODO Auto-generated constructor stub

}

SoundManager::~SoundManager() {
	// TODO Auto-generated destructor stub
}

void SoundManager::start() {
	if(!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG)) {
		SDL_Log("An error ocurred initializing Mixer");
	}

	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16;
	int audio_channels = 2;
	int audio_buffers = 4096;

	Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);
}


void SoundManager::loadMusic(std::string name) {
	if( musics[name] )
		return;

	std::string filename = "assets/music/"+name;
	SDL_Log("%s", filename.c_str());
	Mix_Music *music = Mix_LoadMUS( filename.c_str() );
	musics[ name ] = music;
}

void SoundManager::loadSound(std::string name) {
	if( sounds[name] )
		return;

	std::string filename = "assets/sounds/"+name;
	Mix_Chunk *sound = Mix_LoadWAV( filename.c_str() );
	sounds[ name ] = sound;
}

void SoundManager::playMusic(std::string name, int loops, int fade_in) {
	if( !musics[name] )
		return;

	Mix_FadeInMusic( musics[name], loops, fade_in );
}


void SoundManager::stopMusic(int fade_out) {
	Mix_FadeOutMusic(fade_out);
}


void SoundManager::playSound(std::string name, int loops) {
	if( !musics[name] )
		return;

	Mix_PlayChannel( -1, sounds[name], loops );
}
