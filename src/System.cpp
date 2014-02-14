/*
 * System.cpp
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#include <vector>
#include "System.h"
#include "Entity.h"

System::System(std::vector<Entity *>* _gameObjects) {
	gameObjects = _gameObjects;
}


System::~System() {

}


void System::registerEntity(Entity *entity) {

}


void System::update(Uint32 dt) {

}

void System::start() {

	for(std::vector<Entity*>::iterator entity = gameObjects->begin(); entity != gameObjects->end(); ++entity) {
		registerEntity( *entity );
	}

}

void System::end() {

}
