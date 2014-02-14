/*
 * MovementSystem.h
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef MOVEMENTSYSTEM_H_
#define MOVEMENTSYSTEM_H_

#include "System.h"


class MovementSystem : public System {
public:
	using System::start;
	using System::end;
	using System::registerEntity;

	MovementSystem(std::vector<Entity*>* v);
	virtual ~MovementSystem();

	void update(Uint32 dt);
};

#endif /* MOVEMENTSYSTEM_H_ */
