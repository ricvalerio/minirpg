/*
 * SoundManager.h
 *
 *  Created on: 16 de Jan de 2014
 *      Author: Ricardo
 */

#ifndef SOUNDMANAGER_H_
#define SOUNDMANAGER_H_

#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


class SoundManager {
public:
	static std::map<std::string, Mix_Chunk *> sounds;
	static std::map<std::string, Mix_Music *> musics;


	SoundManager();
	virtual ~SoundManager();

	static void start();

	static void loadMusic(std::string);
	static void loadSound(std::string);

	static void stopMusic(int = 2000);
	static void playMusic(std::string, int = -1, int = 3000);
	static void playSound(std::string, int = 0);
};

#endif /* SOUNDMANAGER_H_ */
