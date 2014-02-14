/*
 * SystemManager.h
 *
 *  Created on: 19 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef SYSTEMMANAGER_H_
#define SYSTEMMANAGER_H_

#include <SDL2/SDL.h>
#include <vector>
#include <typeinfo>
#include <tr1/unordered_map>


class Entity;
class System;

class SystemManager {
public:
	SystemManager(std::vector<Entity *>*);
	virtual ~SystemManager();

	std::tr1::unordered_map<const std::type_info*, System *> systems_map;
	std::vector<Entity *>* gameObjects;
	std::vector<System*> systems;

	template <typename T>
	void registerSystem(T *system) {
		systems.push_back( system );
		systems_map[ &typeid(T) ] = system;
	}

	void registerEntity(Entity *);

	template <typename T>
	T *getSystem() {
		return (T *) systems_map[ &typeid(T) ];
	}


	void start();
	void update(Uint32);
	void end();
};

#endif /* SYSTEMMANAGER_H_ */
