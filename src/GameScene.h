/*
 * GameScene.h
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include <SDL2/SDL.h>
#include <vector>
#include "Entity.h"
#include "SystemManager.h"

class GameScene {
public:
	bool running;

	std::vector<Entity *>* gameObjects;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SystemManager* systemManager;

	GameScene(SDL_Window*);
	virtual ~GameScene();
};

#endif /* GAMESCENE_H_ */
