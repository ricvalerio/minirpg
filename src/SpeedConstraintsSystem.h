/*
 * SpeedConstraintsSystem.h
 *
 *  Created on: 20 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef SPEEDCONSTRAINTSSYSTEM_H_
#define SPEEDCONSTRAINTSSYSTEM_H_

#include <SDL2/SDL.h>
#include "System.h"

class SpeedConstraintsSystem: public System {
public:
	using System::start;
	using System::end;
	using System::registerEntity;


	SpeedConstraintsSystem(std::vector<Entity *>* _gameObjects);
	virtual ~SpeedConstraintsSystem();

	void update(Uint32);
};

#endif /* SPEEDCONSTRAINTSSYSTEM_H_ */
