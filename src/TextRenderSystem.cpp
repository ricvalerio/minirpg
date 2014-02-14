/*
 * TextRenderSystem.cpp
 *
 *  Created on: 31 de Dez de 2013
 *      Author: Ricardo
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#include "Components.h"
#include "Entity.h"
#include "TextRenderSystem.h"
#include "AssetManager.h"
#include "RenderHelper.h"
#include "Helper.h"


TextRenderSystem::TextRenderSystem(std::vector<Entity *>* _gameObjects, SDL_Renderer* _renderer) : System(_gameObjects) {
	renderer = _renderer;
}

TextRenderSystem::~TextRenderSystem() {

}


void TextRenderSystem::start() {
	System::start();
}


void TextRenderSystem::registerEntity(Entity *entity) {
	Name* name = entity->getComponent<Name>();

	if(name == NULL)
		return;

	SDL_Color color={255, 255, 255};

	names[ name->name ] = AssetManager::createText("nameplates", name->name, color);
}


void TextRenderSystem::update(Uint32 dt) {
	SDL_Rect *srcRect = NULL, *dstRect = NULL;
	Sprite *sprite;
	Name *name;
	Position *position, *cameraPosition;

	Entity *camera = getActiveCamera(gameObjects);
	if(!camera)
		return;

	cameraPosition = ((Entity *) camera->getComponent<FollowCamera>()->target)->getComponent<Position>();

	srcRect = new SDL_Rect();
	dstRect = new SDL_Rect();


	SDL_Color colorw = {255, 255, 255};

	for(std::vector<Entity*>::iterator entity = gameObjects->begin(); entity != gameObjects->end(); ++entity) {
		name = (*entity)->getComponent<Name>();
		sprite = (*entity)->getComponent<Sprite>();
		position = (*entity)->getComponent<Position>();

		if(name == NULL || name->name == "" || sprite == NULL || position == NULL)
			continue;

		srcRect->x = 0;
		srcRect->y = 0;
		srcRect->w = names[ name->name ]->width;
		srcRect->h = names[ name->name ]->height;

		dstRect->w = srcRect->w;
		dstRect->h = srcRect->h;

		dstRect->x = (position->x - dstRect->w/2) - cameraPosition->x + 400;
		dstRect->y = (position->y - sprite->height - 20) - cameraPosition->y + 400;

		SDL_RenderCopy(renderer, names[ name->name ]->texture, srcRect, dstRect);
	}
}

void TextRenderSystem::end() {
}
