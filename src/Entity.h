/*
 * Entity.h
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "ComponentCollection.h"
#include "SystemManager.h"


class SystemManager;

class Entity : public ComponentCollection {
public:
	static int _instanceid;
	int id;

	static SystemManager *_systemManager;

	Entity();
	virtual ~Entity();

	void registerMe();
};

#endif /* ENTITY_H_ */
