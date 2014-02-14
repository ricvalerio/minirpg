/*
 * SpeedConstraintsSystem.cpp
 *
 *  Created on: 20 de Dez de 2013
 *      Author: Ricardo
 */

#include <SDL2/SDL.h>
#include <vector>
#include "Components.h"
#include "SpeedConstraintsSystem.h"

SpeedConstraintsSystem::SpeedConstraintsSystem(std::vector<Entity *>* _gameObjects) : System(_gameObjects) {
	// TODO Auto-generated constructor stub

}

SpeedConstraintsSystem::~SpeedConstraintsSystem() {
	// TODO Auto-generated destructor stub
}


void SpeedConstraintsSystem::update(Uint32 dt) {
	Movement* movement;
	SpeedConstraints* constraint;

	for(std::vector<Entity*>::iterator entity = gameObjects->begin(); entity != gameObjects->end(); ++entity) {
		movement = (*entity)->getComponent<Movement>();
		constraint = (*entity)->getComponent<SpeedConstraints>();

		if(constraint == NULL || movement == NULL)
			continue;

		if(constraint->x != -1) {
			if(movement->vx < -constraint->x)
				movement->vx = -constraint->x;
			else if(movement->vx > constraint->x)
				movement->vx = constraint->x;
		}

		if(constraint->y != -1) {
			if(movement->vy < -constraint->y)
				movement->vy = -constraint->y;
			else if(movement->vy > constraint->y)
				movement->vy = constraint->y;
		}
	}
}


