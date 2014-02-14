/*
 * RenderSystem.cpp
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <algorithm>
#include <exception>

#include "Components.h"
#include "Entity.h"
#include "RenderSystem.h"
#include "Helper.h"
#include "AssetManager.h"


int RenderSystem::cutscene_alpha;
int RenderSystem::fade_alpha;


bool zCompare(Entity *e1, Entity *e2) {
	Position *p1, *p2;

	p1 = e1->getComponent<Position>();
	p2 = e2->getComponent<Position>();

	if(p1->depth == p2->depth) {
		return p1->y < p2->y;
	} else {
		return p1->depth < p2->depth;
	}
}



void WorldChunk::setBounds(float _l, float _t, float _r, float _b) {
	left = _l;
	top = _t;
	right = _r;
	bottom = _b;
}

void WorldChunk::generate(std::vector<Entity *>* gameObjects) {
	entities.clear();

	for(std::vector<Entity *>::iterator iter = gameObjects->begin(); iter != gameObjects->end(); iter++) {
		Entity *gameObject = *iter;
		Position *position = gameObject->getComponent<Position>();
		Sprite *sprite = gameObject->getComponent<Sprite>();

		if(position == NULL || sprite == NULL)
			continue;

		if(position->x > left && position->x < right
				&& position->y > top && position->y < bottom) {
			entities.push_back(gameObject);
		}
	}
}

void WorldChunk::sort() {
	std::sort( entities.begin(), entities.end(), zCompare);
}


RenderSystem::RenderSystem(std::vector<Entity *>* _gameObjects, SDL_Renderer* _renderer) : System(_gameObjects) {
	renderer = _renderer;

	RenderSystem::cutscene_alpha = 0;
	RenderSystem::fade_alpha = 0;

}

RenderSystem::~RenderSystem() {

}


void RenderSystem::registerEntity(Entity *entity) {
	Sprite* sprite = entity->getComponent<Sprite>();

	if(sprite == NULL)
		return;

	TextureInfo *texture = AssetManager::getImageTexture(sprite->filename);
	sprite->total_width = texture->width;
	sprite->total_height = texture->height;
	sprite->selectFrame( sprite->current_frame );

}


void RenderSystem::update(Uint32 dt) {
	SDL_Rect *srcRect, dstRect;

	Entity *c = getActiveCamera(gameObjects);

	// if there is no camera, nothing to do
	if(!c)
		return;

	Position *cameraPosition = ((Entity *) c->getComponent<FollowCamera>()->target)->getComponent<Position>();

	chunk.setBounds(
			cameraPosition->x - 500.0f,
			cameraPosition->y - 400.0f,
			cameraPosition->x + 500.0f,
			cameraPosition->y + 400.0f
			);

	chunk.generate( gameObjects );
	chunk.sort();

	for(std::vector<Entity*>::iterator entity = chunk.entities.begin(); entity != chunk.entities.end(); ++entity) {
		Sprite *sprite = (*entity)->getComponent<Sprite>();
		Position *position = (*entity)->getComponent<Position>();

		if(sprite == NULL || position == NULL)
			continue;

		srcRect = sprite->getFrame();

		// absolute coordinates
		dstRect.x = position->x - srcRect->w/2;
		dstRect.y = position->y - srcRect->h + position->z;
		dstRect.w = srcRect->w;
		dstRect.h = srcRect->h;

		// adjust to camera position and screen offsets
		dstRect.x += -cameraPosition->x + 400;
		dstRect.y += -cameraPosition->y + 400;

		SDL_RenderCopy(renderer, AssetManager::getImageTexture( sprite->filename )->texture, srcRect, &dstRect);
	}




	if(cutscene_alpha > 0) {

		SDL_Rect trect = {0, 0, 800, 100};
		SDL_Rect brect = {0, 500, 800, 100};

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, cutscene_alpha);
		SDL_RenderFillRect(renderer, &trect);
		SDL_RenderFillRect(renderer, &brect);

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

	}
}

