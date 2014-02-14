/*
 * GravitySystem.cpp
 *
 *  Created on: 20 de Dez de 2013
 *      Author: Ricardo
 */

#include <SDL2/SDL.h>
#include "GravitySystem.h"

GravitySystem::GravitySystem(std::vector<Entity *>* _gameObjects) : System(_gameObjects) {

}

GravitySystem::~GravitySystem() {
	// TODO Auto-generated destructor stub
}

void GravitySystem::update(Uint32 dt) {
	Movement* movement;
	Gravity* gravity;

	for(std::vector<Entity*>::iterator entity = gameObjects->begin(); entity != gameObjects->end(); ++entity) {
		gravity = (*entity)->getComponent<Gravity>();
		movement = (*entity)->getComponent<Movement>();

		if(gravity == NULL || movement == NULL)
			continue;

		movement->az += gravity->acceleration;
	}
}

