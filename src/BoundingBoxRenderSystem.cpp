/*
 * BoundingBoxRenderSystem.cpp
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Components.h"
#include "Entity.h"
#include "BoundingBoxRenderSystem.h"


Entity *BoundingBoxRenderSystem::getActiveCamera() {
	for(std::vector<Entity*>::iterator entity = gameObjects->begin(); entity != gameObjects->end(); ++entity)
		if((*entity)->getComponent<ActiveCamera>() != NULL)
			return *entity;

	return NULL;
}


BoundingBoxRenderSystem::BoundingBoxRenderSystem(std::vector<Entity *>* _gameObjects, SDL_Renderer* _renderer) : System(_gameObjects) {
	renderer = _renderer;
}


BoundingBoxRenderSystem::~BoundingBoxRenderSystem() {

}


void BoundingBoxRenderSystem::update(Uint32 dt) {
	SDL_Rect rect;
	Position *position, *cameraPosition;
	BoundingBox *box;

	Entity *camera = getActiveCamera();
	cameraPosition = ((Entity *) camera->getComponent<FollowCamera>()->target)->getComponent<Position>();


	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for(std::vector<Entity*>::iterator entity = gameObjects->begin(); entity != gameObjects->end(); ++entity) {
		position = (*entity)->getComponent<Position>();
		box = (*entity)->getComponent<BoundingBox>();

		if(position == NULL || box == NULL)
			continue;

		rect.x = position->x + box->x - cameraPosition->x + 400;;
		rect.y = position->y + box->y - cameraPosition->y + 400;;
		rect.w = box->w;
		rect.h = box->h;

		SDL_RenderDrawRect(renderer, &rect);

	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

}
