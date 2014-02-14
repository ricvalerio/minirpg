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
#include "SpeechRenderSystem.h"
#include "AssetManager.h"
#include "RenderHelper.h"
#include "Helper.h"


SpeechRenderSystem::SpeechRenderSystem(std::vector<Entity *>* _gameObjects, SDL_Renderer* _renderer) : System(_gameObjects) {
	renderer = _renderer;
}

SpeechRenderSystem::~SpeechRenderSystem() {

}


void SpeechRenderSystem::start() {
	System::start();
}


void SpeechRenderSystem::registerEntity(Entity *entity) {
	Name* name = entity->getComponent<Name>();

	if(name == NULL)
		return;

	SDL_Color color={255, 255, 255};

	names[ name->name ] = AssetManager::createText("nameplates", name->name, color);
}


void SpeechRenderSystem::update(Uint32 dt) {
	SDL_Rect *srcRect = NULL, *dstRect = NULL;
	Sprite *sprite;
	Name *name;
	Position *position, *cameraPosition;
	ChatBubble *chatbubble;

	Entity *camera = getActiveCamera(gameObjects);
	if(!camera)
		return;

	cameraPosition = ((Entity *) camera->getComponent<FollowCamera>()->target)->getComponent<Position>();

	srcRect = new SDL_Rect();
	dstRect = new SDL_Rect();


	SDL_Color colorw = {255, 255, 255};

	for(std::vector<Entity*>::iterator entity = gameObjects->begin(); entity != gameObjects->end(); ++entity) {
		sprite = (*entity)->getComponent<Sprite>();
		position = (*entity)->getComponent<Position>();
		chatbubble = (*entity)->getComponent<ChatBubble>();

		if(!sprite || !position || !chatbubble)
			continue;


		if( !names[ chatbubble->text1 ] ) {
			names[ chatbubble->text1 ] = AssetManager::createText( "text", chatbubble->text1, colorw );
		}

		if( !names[ chatbubble->text2 ] ) {
			names[ chatbubble->text2 ] = AssetManager::createText( "text", chatbubble->text2, colorw );
		}

		int b = 15;
		srcRect->x = 0;
		srcRect->y = 0;
		srcRect->w = names[ chatbubble->text1 ]->width;
		srcRect->h = names[ chatbubble->text1 ]->height;
		if(chatbubble->text2 != "") {
			srcRect->h += b + names[ chatbubble->text2 ]->height;
		}

		dstRect->w = srcRect->w + b*2;
		dstRect->h = srcRect->h + b*2;
		dstRect->x = (position->x - dstRect->w/2) - cameraPosition->x + 400;
		dstRect->y = (position->y - sprite->height - 60) - cameraPosition->y + 400 -b*2;
		if(chatbubble->text2 != "") {
			dstRect->y -= b + names[ chatbubble->text2 ]->height;
		}

		RenderHelper::drawBox( dstRect, b );

		dstRect->x += b;
		dstRect->y += b;
		dstRect->w = names[ chatbubble->text1 ]->width;
		dstRect->h = names[ chatbubble->text1 ]->height;

		if(chatbubble->text1 != "")
			SDL_RenderCopy(renderer, names[ chatbubble->text1 ]->texture, srcRect, dstRect);

		if(chatbubble->text2 != "") {
			dstRect->y += b + names[ chatbubble->text1 ]->height;
			dstRect->w = names[ chatbubble->text2 ]->width;
			dstRect->h = names[ chatbubble->text2 ]->height;
			SDL_RenderCopy(renderer, names[ chatbubble->text2 ]->texture, srcRect, dstRect);
		}
	}

}

void SpeechRenderSystem::end() {
}
