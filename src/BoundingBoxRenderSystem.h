/*
 * BoundingBoxRenderSystem.h
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef BOUNDINGBOXRENDERSYSTEM_H_
#define BOUNDINGBOXRENDERSYSTEM_H_

#include <vector>
#include <SDL2/SDL.h>
#include "Entity.h"
#include "Components.h"
#include "System.h"

class BoundingBoxRenderSystem : public System {
public:
	using System::start;
	using System::end;
	using System::registerEntity;

	SDL_Renderer* renderer;

	BoundingBoxRenderSystem(std::vector<Entity*>* v, SDL_Renderer* r);
	virtual ~BoundingBoxRenderSystem();

	Entity *getActiveCamera();
	void update(Uint32);
};

#endif /* BOUNDINGBOXRENDERSYSTEM_H_ */
