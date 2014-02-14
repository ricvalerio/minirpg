/*
 * TextRenderSystem.h
 *
 *  Created on: 31 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef TEXTRENDERSYSTEM_H_
#define TEXTRENDERSYSTEM_H_

#include <map>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Entity.h"
#include "Components.h"
#include "System.h"
#include "AssetManager.h"


class TextRenderSystem : public System {
public:
	SDL_Renderer* renderer;
	std::map<std::string, TextureInfo *> names;

	TextRenderSystem(std::vector<Entity*>* v, SDL_Renderer* r);
	virtual ~TextRenderSystem();

	void registerEntity(Entity *);
	void start();
	void update(Uint32);
	void end();
};

#endif /* TEXTRENDERSYSTEM_H_ */
