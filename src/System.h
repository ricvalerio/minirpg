/*
 * System.h
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <vector>
#include "IComponent.h"
#include "Entity.h"

class Entity;


class System {
public:
	//std::vector<IComponent*> nodes;
	std::vector<Entity*>* gameObjects;

	System(std::vector<Entity *>*);
	virtual ~System();

	virtual void registerEntity(Entity *);
	virtual void start();
	virtual void update(Uint32);
	virtual void end();
};

#endif /* SYSTEM_H_ */
