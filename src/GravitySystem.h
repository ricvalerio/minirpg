/*
 * GravitySystem.h
 *
 *  Created on: 20 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef GRAVITYSYSTEM_H_
#define GRAVITYSYSTEM_H_

#include <SDL2/SDL.h>
#include <vector>
#include "System.h"
#include "Entity.h"


class GravitySystem : public System {
public:
	using System::start;
	using System::end;
	using System::registerEntity;


	GravitySystem(std::vector<Entity *>* _gameObjects);
	virtual ~GravitySystem();

	void update(Uint32 dt);
};

#endif /* GRAVITYSYSTEM_H_ */
