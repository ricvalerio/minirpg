/*
 * EntityFactory.h
 *
 *  Created on: 29 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef ENTITYFACTORY_H_
#define ENTITYFACTORY_H_


#include "Components.h"
#include "Entity.h"


class EntityParam {
public:
	bool _bool;
	double _double;
	std::string _string;
};


class EntityComponent {
public:
	std::string name;

	std::map<std::string, EntityParam*> params;
};

// maps the blueprints for types of entities
class EntityType {
public:
	std::map<std::string, EntityComponent*> components;
};


class EntityFactory {
public:
	static std::map<std::string, EntityType *> entityTypes;
	static std::map<std::string, Animation *> anims;
	static std::map<std::string, AnimationSet *> animationSets;

	static void loadMetaData( const char *);
	static void loadAnimations( const char *);
	static void loadEntities( const char * );

	static void createComponent(Entity *, EntityComponent *);

	static Entity *createFromType(std::string, EntityType *);
	static Entity *createTile(float x, float y, float z, int tile);
};

#endif /* ENTITYFACTORY_H_ */
