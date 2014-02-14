/*
 * CollisionSystem.h
 *
 *  Created on: 20 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef COLLISIONSYSTEM_H_
#define COLLISIONSYSTEM_H_

#include <SDL2/SDL.h>
#include "System.h"


class CollisionSystem : public System {
public:
	using System::start;
	using System::end;
	using System::registerEntity;


	CollisionSystem(std::vector<Entity*>* _gameObjects);
	virtual ~CollisionSystem();

	void update(Uint32);
};

#endif /* COLLISIONSYSTEM_H_ */
