/*
 * SpeechRenderSystem.h
 *
 *  Created on: 30 de Jan de 2014
 *      Author: Ricardo
 */

#ifndef SPEECHRENDERSYSTEM_H_
#define SPEECHRENDERSYSTEM_H_

#include <map>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Entity.h"
#include "Components.h"
#include "System.h"
#include "AssetManager.h"


class SpeechRenderSystem : public System {
public:
	SDL_Renderer* renderer;
	std::map<std::string, TextureInfo *> names;

	SpeechRenderSystem(std::vector<Entity*>* v, SDL_Renderer* r);
	virtual ~SpeechRenderSystem();

	void registerEntity(Entity *);
	void start();
	void update(Uint32);
	void end();
};

#endif /* SPEECHRENDERSYSTEM_H_ */
