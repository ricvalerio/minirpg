/*
 * SystemManager.cpp
 *
 *  Created on: 19 de Dez de 2013
 *      Author: Ricardo
 */

#include <vector>
#include "SystemManager.h"
#include "System.h"


SystemManager::SystemManager(std::vector<Entity *>* _gameObjects) {
	gameObjects = _gameObjects;
}


SystemManager::~SystemManager() {
}


void SystemManager::start() {
	SDL_Log("System Manager start cycle\n");

	for(std::vector<System*>::iterator system = systems.begin(); system != systems.end(); ++system) {
		(*system)->start();
	}
}



void SystemManager::registerEntity(Entity *entity) {

	for(std::vector<System*>::iterator system = systems.begin(); system != systems.end(); ++system) {
		(*system)->registerEntity(entity);
	}

	gameObjects->push_back(entity);
}


void SystemManager::update(Uint32 dt) {
	for(std::vector<System*>::iterator system = systems.begin(); system != systems.end(); ++system) {
		(*system)->update(dt);
	}

}



void SystemManager::end() {
	SDL_Log("System Manager end cycle\n");

	for(std::vector<System*>::iterator system = systems.begin(); system != systems.end(); ++system) {
		(*system)->end();
	}
}





