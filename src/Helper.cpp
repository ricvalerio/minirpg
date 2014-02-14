/*
 * Helper.cpp
 *
 *  Created on: 10 de Jan de 2014
 *      Author: Ricardo
 */

#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Components.h"
#include "Entity.h"
#include "Helper.h"
#include "HudRenderSystem.h"
#include "AssetManager.h"
#include "EntityFactory.h"
#include "Event.h"
#include "ScriptSystem.h"


Entity *activeCamera = NULL;
std::queue<Event *> events;

void createFireball(float x, float y, float vx, float vy, float depth) {
	std::string direction;
	if(vx > 0) direction = "right";
	else if(vx < 0) direction = "left";
	else if(vy < 0) direction = "up";
	else if(vy > 0) direction = "down";

	Entity *entity = EntityFactory::createFromType("fireball", NULL);
	entity->addComponent( new Position( x, y, depth, -50.0f ));
	entity->addComponent( new Movement(vx, vy, 0.0f ));
	entity->getComponent<Animations>()->setCurrent(direction);
	entity->registerMe();

	subs["collision"]->push_back(entity);
}



void createBag(float x, float y, float depth) {
	Entity *bag = new Entity();
	bag->addComponent( new Name("treasure") );
	bag->addComponent( new Position( x, y, depth ));
	bag->addComponent( new Movement(0.0f, 0.0f, -10.0f ));
	bag->addComponent( new SpeedNeutralizer(1.0f, 1.0f) );
	bag->addComponent( new Gravity(1.0f) );
	bag->addComponent( new BoundingBox(-16.0f, -32.0f, 32.0f, 32.0f) );
	bag->addComponent( new Sprite("bag.png", 32, 32, 0, 0, 0, 0) );
	bag->addComponent( new Container() );
	bag->registerMe();

	Entity *entity;

	entity = new Entity();
	entity->addComponent( new Name("Health Potion") );
	entity->addComponent( new Sprite("potions.png", 16, 32, 0, 0, 0, 0, 5) );
	entity->registerMe();
	bag->getComponent<Container>()->add( entity );

	entity = new Entity();
	entity->addComponent( new Name("Mana Potion") );
	entity->addComponent( new Sprite("potions.png", 16, 32, 0, 0, 0, 0, 3) );
	entity->registerMe();
	bag->getComponent<Container>()->add( entity );

	entity = new Entity();
	entity->addComponent( new Name("Mana Potion") );
	entity->addComponent( new Sprite("potions.png", 16, 32, 0, 0, 0, 0, 2) );
	entity->registerMe();
	bag->getComponent<Container>()->add( entity );

	entity = new Entity();
	entity->addComponent( new Name("Mana Potion") );
	entity->addComponent( new Sprite("potions.png", 16, 32, 0, 0, 0, 0, 1) );
	entity->registerMe();
	bag->getComponent<Container>()->add( entity );

	HudRenderSystem::createContainer( &bag->getComponent<Container>()->container );
}


Entity *getPlayer(std::vector<Entity *>* _entities) {
	for(std::vector<Entity*>::iterator entity = _entities->begin(); entity != _entities->end(); ++entity)
		if((*entity)->getComponent<Player>() != NULL)
			return *entity;

	return NULL;
}


Entity *getByID(std::vector<Entity *>* _entities, int id) {
	for(std::vector<Entity*>::iterator entity = _entities->begin(); entity != _entities->end(); ++entity)
		if((*entity)->id == id)
			return *entity;

	return NULL;
}


Entity *getActiveCamera(std::vector<Entity *> *_entities) {
	if(activeCamera)
		return activeCamera;

	for(std::vector<Entity*>::iterator entity = _entities->begin(); entity != _entities->end(); ++entity) {
		if((*entity)->getComponent<ActiveCamera>() != NULL) {
			activeCamera = *entity;
			break;
		}
	}

	return activeCamera;
}
