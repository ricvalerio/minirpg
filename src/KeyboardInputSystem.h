/*
 * KeyboardInputSystem.h
 *
 *  Created on: 19 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef KEYBOARDINPUTSYSTEM_H_
#define KEYBOARDINPUTSYSTEM_H_

#include <SDL2/SDL.h>
#include "System.h"

class KeyboardInputSystem : public System {
public:
	using System::start;
	using System::end;
	using System::registerEntity;


	KeyboardInputSystem(std::vector<Entity *>* _gameObjects);
	virtual ~KeyboardInputSystem();

	void update(Uint32);
};

#endif /* KEYBOARDINPUTSYSTEM_H_ */
