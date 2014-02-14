/*
 * KeyboardInputSystem.cpp
 *
 *  Created on: 19 de Dez de 2013
 *      Author: Ricardo
 */

#include <SDL2/SDL.h>
#include "KeyboardInputSystem.h"
#include "Components.h"


KeyboardInputSystem::KeyboardInputSystem(std::vector<Entity *>* _gameObjects) : System(_gameObjects) {

}

KeyboardInputSystem::~KeyboardInputSystem() {

}


void KeyboardInputSystem::update(Uint32 dt) {
	KeyboardInput* keyboard;
	Movement* movement;
	Player *player;


	for(std::vector<Entity*>::iterator entity = gameObjects->begin(); entity != gameObjects->end(); ++entity) {
		player = (*entity)->getComponent<Player>();

		if(player == NULL)
			continue;

		keyboard = (*entity)->getComponent<KeyboardInput>();
		movement = (*entity)->getComponent<Movement>();

		if(keyboard == NULL || movement == NULL)
			continue;

		if(keyboard->freeze)
			continue;

		SDL_PumpEvents();
		const Uint8 *state = SDL_GetKeyboardState(NULL);

		bool up = state[ keyboard->getKey("up")];
		bool down = state[ keyboard->getKey("down")];
		bool left = state[ keyboard->getKey("left")];
		bool right = state[ keyboard->getKey("right")];
		bool jump = state[ keyboard->getKey("jump")];
		bool shoot = state[ keyboard->getKey("shoot")];

		if(shoot && (*entity)->getComponent<ActionShoot>()
				&& (*entity)->getComponent<ActionShoot>()->isReady()
				&& !(*entity)->getComponent<ActionShoot>()->active) {
			(*entity)->getComponent<ActionShoot>()->perform();

		}

		if(jump &&
				(*entity)->getComponent<ActionJump>() != NULL &&
				!(*entity)->getComponent<ActionJump>()->active
				&& (*entity)->getComponent<Position>()->z == 0.0f) {
			Action* action = (*entity)->getComponent<ActionJump>();
			if(action != NULL && action->isReady())
				action->perform();
		}


		if(up ^ down) {
			if(up) {
				Action* action = (*entity)->getComponent<ActionWalkUp>();
				if(action != NULL)
					action->perform();
			} else if(down) {
				Action* action = (*entity)->getComponent<ActionWalkDown>();
				if(action != NULL)
					action->perform();
			}
		} else {
			Action* action = (*entity)->getComponent<ActionWalkUp>();
			if(action != NULL)
				action->stop();

			action = (*entity)->getComponent<ActionWalkDown>();
			if(action != NULL)
				action->stop();

		}


		if(left ^ right) {
			if(left) {
				Action* action = (*entity)->getComponent<ActionWalkLeft>();
				if(action != NULL)
					action->perform();
			} else if(right) {
				Action* action = (*entity)->getComponent<ActionWalkRight>();
				if(action != NULL)
					action->perform();
			}
		} else {
			Action* action = (*entity)->getComponent<ActionWalkLeft>();
			if(action != NULL)
				action->stop();

			action = (*entity)->getComponent<ActionWalkRight>();
			if(action != NULL)
				action->stop();

		}



	}

}

