/*
 * RenderSystem.h
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef RENDERSYSTEM_H_
#define RENDERSYSTEM_H_

#include <vector>
#include <SDL2/SDL.h>
#include "Entity.h"
#include "Components.h"
#include "System.h"


class WorldChunk {
public:
	float left, top, right, bottom;

	std::vector<Entity*> entities;

	void setBounds(float, float, float, float);
	void generate(std::vector<Entity *>* );
	void sort();
};



class RenderSystem : public System {
private:
	WorldChunk chunk;

public:
	using System::start;
	using System::end;

	static int cutscene_alpha, fade_alpha;

	SDL_Renderer* renderer;

	RenderSystem(std::vector<Entity*>* v, SDL_Renderer* r);
	virtual ~RenderSystem();

	void registerEntity(Entity *);
	void update(Uint32);
};

#endif /* RENDERSYSTEM_H_ */
