/*
 * ScriptSystem.h
 *
 *  Created on: 5 de Jan de 2013
 *      Author: Ricardo
 */

#ifndef SCRIPTSYSTEM_H_
#define SCRIPTSYSTEM_H_

#include <vector>
#include <SDL2/SDL.h>
#include <lua/lua.hpp>
#include "Entity.h"
#include "Components.h"
#include "System.h"
#include "ActionList.h"


extern std::map<std::string, std::vector<Entity *>*> subs;
extern ActionList actionlist;
extern std::vector<Entity *>* _entities;

class ScriptSystem : public System {
private:
	lua_State *lua_state;
	lua_State *lua_thread;

public:
	ScriptSystem(std::vector<Entity*>* v);
	virtual ~ScriptSystem();

	void addScript(const char *s, bool r = false);
	void registerEntity(Entity *);
	void update(Uint32);
	void start();
	void end();
};

#endif /* RENDERSYSTEM_H_ */
