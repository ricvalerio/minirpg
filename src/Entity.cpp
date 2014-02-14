/*
 * Entity.cpp
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#include "ComponentCollection.h"
#include "Entity.h"
#include "SystemManager.h"


int Entity::_instanceid = 0;
SystemManager *Entity::_systemManager;


Entity::Entity() {
	id = Entity::_instanceid++;
}

Entity::~Entity() {

}

void Entity::registerMe() {
	_systemManager->registerEntity( this );
}

