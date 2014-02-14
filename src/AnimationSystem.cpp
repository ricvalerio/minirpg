/*
 * AnimationSystem.cpp
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Components.h"
#include "Entity.h"
#include "AnimationSystem.h"


AnimationSystem::AnimationSystem(std::vector<Entity *>* _gameObjects) : System(_gameObjects) {
}


AnimationSystem::~AnimationSystem() {

}


void AnimationSystem::update(Uint32 dt) {
	Sprite* sprite;
	Animations* animations;
	Animation* animation;

	for(std::vector<Entity*>::iterator entity = gameObjects->begin(); entity != gameObjects->end(); ++entity) {
		sprite = (*entity)->getComponent<Sprite>();
		animations = (*entity)->getComponent<Animations>();

		if(sprite == NULL || animations == NULL)
			continue;

		if(!animations->isRunning())
			continue;

		animation = animations->getCurrent();

		animation->delta += dt;

		if(animation->delta > animation->fps_time) {
			animation->frame++;
			animation->delta %= animation->fps_time;
		}

		animation->frame %= animation->frames->size();

		sprite->selectFrame(animation->frames->at(animation->frame));

	}
}
