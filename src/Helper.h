/*
 * Helper.h
 *
 *  Created on: 10 de Jan de 2014
 *      Author: Ricardo
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <queue>
#include "Entity.h"
#include "Event.h"

extern Entity *activeCamera;
extern std::queue<Event *> events;


void createFireball(float x, float y, float vx, float vy, float depth);
void createBag(float x, float y, float depth);
Entity *getPlayer(std::vector<Entity *>* _entities);
Entity *getByID(std::vector<Entity *>* _entities, int id);
Entity *getActiveCamera(std::vector<Entity *>* _entities);


#endif /* HELPER_H_ */
