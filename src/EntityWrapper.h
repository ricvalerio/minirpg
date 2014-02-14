/*
 * EntityWrapper.h
 *
 *  Created on: 24 de Jan de 2014
 *      Author: Ricardo
 */

#ifndef ENTITYWRAPPER_H_
#define ENTITYWRAPPER_H_

#include <vector>
#include <map>
#include "Entity.h"
#include "Components.h"
#include <luabind/luabind.hpp>


class EntityWrapper : public Entity {
public:
	EntityWrapper() : Entity() {

	}

	void setPosition(float x, float y);
	void setName(const char *_name);
	const char* getName();
	void applyForce(float x, float y, float z);
	void applyForce(Facing *facing, float speed);
	void teleport(int x, int y);
	void addItem(EntityWrapper *item);
	bool isPlayer();
	bool isSpell();
	bool isAttackable();

	void removeItem(const char *_name);
	bool hasItem(const char *_name);
	void addToInventory(Entity *item);
	void unsubscribeAll();
	bool isContainer();
	void destroy();
	void take_damage(Damage * damageC);
	void speak(int duration, const char *t1);
	void speak(int duration, const char *t1, const char *t2);

};


#endif /* ENTITYWRAPPER_H_ */
