/*
 * Engine.cpp
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "Engine.h"
#include "GameScene.h"

Engine::Engine() {
	renderer = NULL;
	window = NULL;
}


Engine::~Engine() {

}


int Engine::run() {

	int flags = SDL_INIT_EVERYTHING;

	std::cout << "Initializing...\n";

	SDL_SetMainReady();
	SDL_Init( flags );

	std::cout << "Creating window...\n";

	window = SDL_CreateWindow(
			"main",
			300,
			SDL_WINDOWPOS_UNDEFINED,
			800,
			600,
			SDL_WINDOW_OPENGL);


	if (window == NULL) {
		std::cout << "Could not create window: %s\n", SDL_GetError();
		return 1;
	}


	std::cout << "Initializing fonts...\n";

	if(TTF_Init() < 0) {
		std::cout << "Could not initialize TTF.\n";
		return 1;
	}

	GameScene* gameScene = new GameScene(window);

	TTF_Quit();
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}
