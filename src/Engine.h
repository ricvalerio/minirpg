/*
 * Engine.h
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <SDL2/SDL.h>

class Engine {
public:
	Engine();
	virtual ~Engine();

	SDL_Window *window;
	SDL_Renderer *renderer;

	int run();
};

#endif /* ENGINE_H_ */
