/*
 * PerformActionsSystem.cpp
 *
 *  Created on: 20 de Dez de 2013
 *      Author: Ricardo
 */

#include <SDL2/SDL.h>
#include <vector>
#include "Entity.h"
#include "PerformActionsSystem.h"
#include "Components.h"
#include "Helper.h"


PerformActionsSystem::PerformActionsSystem(std::vector<Entity *>* _gameObjects) : System(_gameObjects) {
	// TODO Auto-generated constructor stub

}

PerformActionsSystem::~PerformActionsSystem() {
	// TODO Auto-generated destructor stub
}


void PerformActionsSystem::update(Uint32 dt) {
	Movement *movement;

	for(std::vector<Entity*>::iterator iter = gameObjects->begin(); iter != gameObjects->end(); ++iter) {
		Entity *entity = *iter;
		movement = entity->getComponent<Movement>();

		if(movement == NULL)
			continue;

		ActionWalkUp *up = entity->getComponent<ActionWalkUp>();
		ActionWalkDown *down = entity->getComponent<ActionWalkDown>();
		ActionWalkLeft *left = entity->getComponent<ActionWalkLeft>();
		ActionWalkRight *right = entity->getComponent<ActionWalkRight>();
		ActionJump *jump = entity->getComponent<ActionJump>();
		ActionShoot *shoot = entity->getComponent<ActionShoot>();
		Facing *facing = entity->getComponent<Facing>();

		if(shoot) {
			if(shoot->timer > 0)
				shoot->timer--;

			if(shoot->timer == 0)
				shoot->setAvailable(true);
		}



		if(	up && up->active ) {
			up->active = false;
			movement->vy -= 2;

			facing->setUp();
		}

		if(	down && down->active ) {
			down->active = false;
			movement->vy += 2;

			if(facing) facing->setDown();
		}

		if(	left && left->active ) {
			left->active = false;
			movement->vx -= 2;

			if(facing) facing->setLeft();
		}

		if(	right && right->active ) {
			right->active = false;
			movement->vx += 2;

			if(facing) facing->setRight();
		}

		if(	jump && jump->active && jump->isReady()) {
			movement->az = -60.0f;

			jump->setAvailable(false);
			jump->stop();
		}


		if( shoot && shoot->isReady() && shoot->active ) {
			shoot->active = false;
			shoot->setAvailable(false);
			shoot->timer = shoot->reload_time;
			events.push( new ActionKeyEvent( entity ) );
		}

		Animations *animations = entity->getComponent<Animations>();

		if(!animations)
			continue;

		if(facing) {

			animations->start();
			if(facing->isFacingUp())
				animations->setCurrent("up");
			else if(facing->isFacingDown())
				animations->setCurrent("down");
			else if(facing->isFacingLeft())
				animations->setCurrent("left");
			else if(facing->isFacingRight())
				animations->setCurrent("right");
			else
				animations->setCurrent("still");

			if(movement->vx == 0 && movement->vy == 0.0f)
				animations->stop();
		} else {

			if(movement->vx == 0) {
				if(movement->vy > 0)
					animations->setCurrent("down");
				else if(movement->vy < 0)
					animations->setCurrent("up");
				else
					animations->setCurrent("still");
			} else if(movement->vx > 0) {
				animations->setCurrent("right");
			} else if(movement->vx < 0) {
				animations->setCurrent("left");
			}
		}

	}

}
