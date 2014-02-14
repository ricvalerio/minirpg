/*
 * PerformActionsSystem.h
 *
 *  Created on: 20 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef PERFORMACTIONSSYSTEM_H_
#define PERFORMACTIONSSYSTEM_H_

#include <SDL2/SDL.h>
#include <vector>
#include "Entity.h"
#include "System.h"

class PerformActionsSystem: public System {
public:
	using System::start;
	using System::end;
	using System::registerEntity;


	PerformActionsSystem(std::vector<Entity *>* _gameObjects);
	virtual ~PerformActionsSystem();


	void update(Uint32);
};

#endif /* PERFORMACTIONSSYSTEM_H_ */
