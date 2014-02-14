/*
 * EntityWrapper.cpp
 *
 *  Created on: 24 de Jan de 2014
 *      Author: Ricardo
 */

#include <string>
#include <vector>
#include <map>
#include "Entity.h"
#include "Components.h"
#include "EntityWrapper.h"
#include "ScriptSystem.h"


void EntityWrapper::setPosition(float x, float y) {
	Position *p = getComponent<Position>();
	if(p) {
		p->x = x;
		p->y = y;
	}
}

const char *EntityWrapper::getName() {
	Name *name = getComponent<Name>();
	if(name) {
		return name->name.c_str();
	}
}

void EntityWrapper::setName(const char *_name) {
	Name *name = getComponent<Name>();
	if(name) {
		name->name = "";
	}
}

void EntityWrapper::applyForce(float x, float y, float z) {
	Movement *m = getComponent<Movement>();

	if(m) {
		m->ax += x;
		m->ay += y;
		m->az += z;
	}
}


void EntityWrapper::applyForce(Facing *facing, float speed) {
	Movement *movement = getComponent<Movement>();
	if(!movement || !facing)
		return;

	if(facing->isFacingUp()) {
		movement->ay += -speed;
	} else if(facing->isFacingDown()) {
		movement->ay += speed;
	} else if(facing->isFacingLeft()) {
		movement->ax += -speed;
	} else if(facing->isFacingRight()) {
		movement->ax += speed;
	}
}

void EntityWrapper::teleport(int x, int y) {
	setPosition(x, y);
}

void EntityWrapper::addItem(EntityWrapper *item) {
	Container *container = this->getComponent<Container>();

	if(container)
		container->add( (Entity *) item );
}

bool EntityWrapper::isPlayer() {
	if( getComponent<Player>() )
		return true;

	return false;
}

bool EntityWrapper::isSpell() {
	if( getComponent<Damage>() )
		return true;

	return false;
}

bool EntityWrapper::isAttackable() {
	if( getComponent<Hostile>() )
		return true;

	return false;
}

void EntityWrapper::removeItem(const char *_name) {
	Inventory *inv = getComponent<Inventory>();
	std::string item_name(_name);

	if( !inv )
		return;

	std::vector<Entity *>* items = inv->getAll();

	for(std::vector<Entity *>::iterator iter = items->begin(); iter != items->end(); ++iter) {
		Name *name = (*iter)->getComponent<Name>();
		if(name && name->name == item_name) {
			items->erase( iter-- );
		}
	}
}

bool EntityWrapper::hasItem(const char *_name) {
	Inventory *inv = getComponent<Inventory>();
	std::string item_name(_name);

	if( !inv )
		return false;

	std::vector<Entity *>* items = inv->getAll();

	for(std::vector<Entity *>::iterator iter = items->begin(); iter != items->end(); ++iter) {
		Name *name = (*iter)->getComponent<Name>();
		if(name && name->name == item_name)
			return true;
	}

	return false;
}


void EntityWrapper::addToInventory(Entity *item) {
	Inventory *inv = getComponent<Inventory>();

	if( inv ) {
		if(item->getComponent<Container>()) {
			std::vector<Entity *>* items = item->getComponent<Container>()->getAll();

			for(std::vector<Entity *>::iterator iter = items->begin(); iter != items->end(); ++iter) {
				if((*iter)->getComponent<Name>())
					SDL_Log("picked item: %s", (*iter)->getComponent<Name>()->name.c_str());

				inv->add( *iter );
				items->erase( iter-- );
			}
		} else {
			inv->add( item );
		}

	}
}


void EntityWrapper::unsubscribeAll() {
	for(std::map<std::string, std::vector<Entity *>*>::iterator iter = subs.begin(); iter != subs.end(); ++iter) {
		std::string type = iter->first;
		std::vector<Entity *> *entities = iter->second;

		entities->erase(std::remove( entities->begin(), entities->end(), (Entity *) this), entities->end());
	}
}


bool EntityWrapper::isContainer() {
	if( getComponent<Container>() ) {
		return true;
	}

	return false;
}

void EntityWrapper::destroy() {
	_entities->erase(std::remove( _entities->begin(), _entities->end(), (Entity *) this), _entities->end());
	unsubscribeAll();
}

void EntityWrapper::take_damage(Damage * damageC) {
	Health *health = getComponent<Health>();

	if(health) {
		health->takeDamage(damageC->amount);
	}
}


void EntityWrapper::speak(int duration, const char *t1) {
	speak(duration, t1, NULL);
}

void EntityWrapper::speak(int duration, const char *t1, const char *t2) {
	std::string text1(t1);
	std::string text2(t2? t2: "");


	actionlist.push_back( new SpeechAction( &actionlist, duration, (Entity *) this, text1, text2) );
}

