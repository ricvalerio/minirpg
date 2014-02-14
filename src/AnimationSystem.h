/*
 * AnimationSystem.h
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef ANIMATIONSYSTEM_H_
#define ANIMATIONSYSTEM_H_

#include <vector>
#include <SDL2/SDL.h>
#include "Entity.h"
#include "Components.h"
#include "System.h"

class AnimationSystem : public System {
public:
	using System::start;
	using System::end;
	using System::registerEntity;

	AnimationSystem(std::vector<Entity*>* v);
	virtual ~AnimationSystem();

	void update(Uint32 dt);
};

#endif /* ANIMATIONSYSTEM_H_ */
