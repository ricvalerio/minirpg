/*
 * HudRenderSystem.cpp
 *
 *  Created on: 2 de Jan de 2014
 *      Author: Ricardo
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Components.h"
#include "Entity.h"
#include "HudRenderSystem.h"
#include "Helper.h"


std::vector<HUDWidget *> HudRenderSystem::widgets;

HudRenderSystem::HudRenderSystem(std::vector<Entity *>* _gameObjects, SDL_Renderer* _renderer) : System(_gameObjects) {
	renderer = _renderer;
}

HudRenderSystem::~HudRenderSystem() {

}


void HudRenderSystem::update(Uint32 dt) {
	SDL_Rect dstRect;
	Health *health;

	int barCounter = 1;

	Entity *player = getPlayer(gameObjects);
	if(!player)
		return;

	health = player->getComponent<Health>();

	dstRect.x = 18;
	dstRect.w = health->max_amount * 2 + 4;
	dstRect.y = barCounter*40-2;
	dstRect.h = 34;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &dstRect);

	dstRect.x = 20;
	dstRect.w = health->amount * 2;
	dstRect.y = barCounter*40;
	dstRect.h = 30;

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
	SDL_RenderFillRect(renderer, &dstRect);
	barCounter++;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	for(std::vector<HUDWidget *>::iterator iter = widgets.begin(); iter != widgets.end(); iter++) {
		HUDWidget *widget = *iter;
		widget->update();
		widget->draw(renderer);
	}

}

