/*
 * MovementSystem.cpp
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#include "Components.h"
#include <vector>
#include "MovementSystem.h"

MovementSystem::MovementSystem(std::vector<Entity*>* _gameObjects) : System(_gameObjects) {

}

MovementSystem::~MovementSystem() {
}


void MovementSystem::update(Uint32 dt) {
	Position* position;
	Movement* movement;
	SpeedNeutralizer* neutralizer;

	for(std::vector<Entity*>::iterator entity = gameObjects->begin(); entity != gameObjects->end(); ++entity) {
		position = (*entity)->getComponent<Position>();
		movement = (*entity)->getComponent<Movement>();

		if(position == NULL || movement == NULL)
			continue;

		movement->vx += movement->ax;
		movement->vy += movement->ay;
		movement->vz += movement->az;

		movement->ax = 0;
		movement->ay = 0;
		movement->az = 0;

		position->x += movement->vx;
		position->y += movement->vy;
		position->z += movement->vz;
		if(position->z > 0.0f) position->z = 0.0f;

		neutralizer = (*entity)->getComponent<SpeedNeutralizer>();
		if(neutralizer != NULL) {
			if(movement->vx > neutralizer->ax)
				movement->vx -= neutralizer->ax;
			else if(movement->vx < -neutralizer->ax)
				movement->vx += neutralizer->ax;
			else
				movement->vx = 0.0f;

			if(movement->vy > neutralizer->ay)
				movement->vy -= neutralizer->ay;
			else if(movement->vy < -neutralizer->ay)
				movement->vy += neutralizer->ax;
			else
				movement->vy = 0.0f;



		}

	}

}
