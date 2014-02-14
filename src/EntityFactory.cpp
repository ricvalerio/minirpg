/*
 * EntityFactory.cpp
 *
 *  Created on: 29 de Dez de 2013
 *      Author: Ricardo
 */

#include <typeinfo>
#include <string>
#include "EntityFactory.h"
#include "Entity.h"
#include "SystemManager.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include <iostream>

#define PARAM_STR(x) ( c->params[x]->_string )
#define PARAM_INT(x) ( (int) c->params[x]->_double )
#define PARAM_FLOAT(x) ( (float) c->params[x]->_double )
#define PARAM_BOOL(x) ( c->params[x]->_bool )


std::map<std::string, EntityType *> EntityFactory::entityTypes;
std::map<std::string, Animation *> EntityFactory::anims;
std::map<std::string, AnimationSet *> EntityFactory::animationSets;


void EntityFactory::loadMetaData(const char *metadata_file) {
	EntityParam *cparam;

	FILE * pFile = fopen(metadata_file , "r");
	rapidjson::FileStream is(pFile);
	rapidjson::Document jsondoc;
	jsondoc.ParseStream<0>(is);


	if(jsondoc.HasParseError()) {
		printf("%s", jsondoc.GetParseError());
		SDL_Delay(2000);
		return;
	}

	for (rapidjson::Value::ConstMemberIterator m = jsondoc["meta-entities"].MemberBegin(); m != jsondoc["meta-entities"].MemberEnd(); ++m) {
	    printf("New entity type: \"%s\"\n", m->name.GetString());
	    EntityType *entityType = new EntityType();

		for (rapidjson::Value::ConstMemberIterator e = m->value.MemberBegin(); e != m->value.MemberEnd(); ++e) {
	    	EntityComponent *entityComponent = new EntityComponent();
	    	entityComponent->name = e->name.GetString();

	    	printf("  Component \"%s\"\n", e->name.GetString());

	    	if(!e->value.IsNull()) {

				printf("    Args: ");

				for (rapidjson::Value::ConstMemberIterator args = e->value.MemberBegin(); args != e->value.MemberEnd(); ++args) {
					int type = args->value.GetType();

					cparam = new EntityParam();

					//{ "Null", "False", "True", "Object", "Array", "String", "Number" };
					switch(type) {
					case 1:
					case 2:
						cparam->_bool = args->value.GetBool();
						break;
					case 5:
						cparam->_string = args->value.GetString();
						break;
					case 6:
						cparam->_double = args->value.GetDouble();
						break;
					}

					entityComponent->params[args->name.GetString()] = cparam;
				}

				printf("\n");
	    	}

	    	entityType->components[ entityComponent->name ] = entityComponent;
	    }

	    entityTypes[m->name.GetString()] = entityType;
	}

	fclose(pFile);
}


void EntityFactory::loadAnimations(const char *animations_def_file) {
	FILE * pFile = fopen( animations_def_file , "r");
	rapidjson::FileStream is(pFile);
	rapidjson::Document jsondoc;
	jsondoc.ParseStream<0>(is);


	if(jsondoc.HasParseError()) {
		printf("%s", jsondoc.GetParseError());
		SDL_Delay(2000);
		return;
	}


	AnimationSet *animations;
	std::vector<int>* vec;

	// animation loading
	for (rapidjson::Value::ConstMemberIterator animgroup = jsondoc["animations"].MemberBegin(); animgroup != jsondoc["animations"].MemberEnd(); ++animgroup) {
	    printf("New animation set: \"%s\"\n", animgroup->name.GetString());
    	animations = new AnimationSet();

    	std::string name;
		for (rapidjson::Value::ConstMemberIterator animsingle = animgroup->value.MemberBegin(); animsingle != animgroup->value.MemberEnd(); ++animsingle) {
	    	name = animsingle->name.GetString();
	    	int fps = animsingle->value["fps"].GetInt();
	    	vec = new std::vector<int>();

	    	printf("  animation: %s\n", name.c_str());

		    for (rapidjson::SizeType i = 0; i < animsingle->value["frames"].Size(); i++) {
		    	vec->push_back(animsingle->value["frames"][i].GetInt());
		    	printf(" %d", animsingle->value["frames"][i].GetInt());
		    }

		    printf("\n");

		    animations->addAnimation(name, new Animation(name, vec, fps));
		}

		animationSets[ animgroup->name.GetString() ] = animations;

		printf("\n");
	}

	fclose(pFile);
}


void EntityFactory::loadEntities( const char *entities_file ) {
	EntityParam *cparam;

	FILE * pFile = fopen(entities_file , "r");
	rapidjson::FileStream is(pFile);
	rapidjson::Document jsondoc;
	jsondoc.ParseStream<0>(is);


	if(jsondoc.HasParseError()) {
		printf("%s", jsondoc.GetParseError());
		return;
	}

	for (rapidjson::Value::ConstMemberIterator m = jsondoc["entities"].MemberBegin(); m != jsondoc["entities"].MemberEnd(); ++m) {
	    printf("New entity of type: \"%s\"\n", m->name.GetString());
	    EntityType *entityParams = new EntityType();


		for (rapidjson::Value::ConstMemberIterator e = m->value.MemberBegin(); e != m->value.MemberEnd(); ++e) {
	    	EntityComponent *entityComponent = new EntityComponent();
	    	entityComponent->name = e->name.GetString();

	    	printf("  Component \"%s\"\n", e->name.GetString());

	    	if(e->value.IsNull())
	    		continue;

	    	printf("    Args: ");

			for (rapidjson::Value::ConstMemberIterator args = e->value.MemberBegin(); args != e->value.MemberEnd(); ++args) {
				int type = args->value.GetType();

				cparam = new EntityParam();

				//{ "Null", "False", "True", "Object", "Array", "String", "Number" };
				switch(type) {
				case 1:
				case 2:
					cparam->_bool = args->value.GetBool();
					break;
				case 5:
					cparam->_string = args->value.GetString();
					break;
				case 6:
					cparam->_double = args->value.GetDouble();
					break;
				}

				entityComponent->params[args->name.GetString()] = cparam;
			}


			printf("\n");

	    	entityParams->components[ entityComponent->name ] = entityComponent;
	    }

		Entity *gameObject = createFromType( m->name.GetString(), entityParams);
		gameObject->registerMe();

	    SDL_Log("  created entity");
	}

	fclose(pFile);
}



Entity *EntityFactory::createFromType(std::string type, EntityType *entityParams) {
	if(!entityParams)
		 entityParams = new EntityType();

	EntityType *entityType = entityTypes[type];
	Entity *gameObject = new Entity();

	for(std::map<std::string,EntityComponent*>::iterator c = entityType->components.begin(); c != entityType->components.end(); ++c) {
		std::string cname = c->first;
		EntityComponent *component = c->second;

		if(!entityParams->components[cname]) {
			EntityComponent *newc = new EntityComponent();
			newc->name = cname;
			entityParams->components[cname] = newc;
		}

		for(std::map<std::string,EntityParam *>::iterator params = component->params.begin(); params != component->params.end(); params++) {
			std::string pname = params->first;
			EntityParam *param = params->second;

			if(!entityParams->components[cname]->params[pname]) {
				entityParams->components[cname]->params[pname] = param;
			}
		}

		createComponent( gameObject, entityParams->components[cname] );
	}

	for(std::map<std::string,EntityComponent*>::iterator c = entityParams->components.begin(); c != entityParams->components.end(); ++c) {
		std::string cname = c->first;
		EntityComponent *component = c->second;

		if(entityType->components[cname] != NULL)
			continue;

		createComponent( gameObject, component );
	}




	return gameObject;
}


void EntityFactory::createComponent(Entity *entity, EntityComponent *c) {

	if(c->name == "id") {

		if(PARAM_INT("id") > 0) {
			entity->id = PARAM_INT("id");
			SDL_Log("setting entity id %d", entity->id);
		}

	} else if(c->name == "Script") {

		entity->addComponent( new Script( PARAM_STR("file") ) );

	} else if(c->name == "Name") {

		entity->addComponent( new Name( PARAM_STR("name") ) );

	} else if(c->name == "Health") {

		entity->addComponent( new Health( PARAM_INT("health") ));

	} else if(c->name == "Damage") {

		entity->addComponent( new Damage( PARAM_INT("amount") ));

	} else if(c->name == "Gravity") {

		entity->addComponent( new Gravity( PARAM_FLOAT("y") ));

	} else if(c->name == "ActionWalkUp") {

		entity->addComponent( new ActionWalkUp() );

	} else if(c->name == "ActionWalkDown") {

		entity->addComponent( new ActionWalkDown() );

	} else if(c->name == "ActionWalkLeft") {

		entity->addComponent( new ActionWalkLeft() );

	} else if(c->name == "ActionWalkRight") {

		entity->addComponent( new ActionWalkRight() );

	} else if(c->name == "ActionJump") {

		entity->addComponent( new ActionJump() );

	} else if(c->name == "ActionShoot") {

		entity->addComponent( new ActionShoot() );

	} else if(c->name == "Facing") {

		entity->addComponent( new Facing(
				PARAM_BOOL("top"),
				PARAM_BOOL("down"),
				PARAM_BOOL("left"),
				PARAM_BOOL("right")
			));

	} else if(c->name == "Pickable") {

		entity->addComponent( new Pickable() );

	} else if(c->name == "NPC") {

		entity->addComponent( new NPC() );

	} else if(c->name == "Hostile") {

		entity->addComponent( new Hostile() );

	} else if(c->name == "Container") {

		entity->addComponent( new Container() );

	} else if(c->name == "Inventory") {

		entity->addComponent( new Inventory() );

	} else if(c->name == "Item") {

		entity->addComponent( new Item() );

	} else if(c->name == "Player") {

		entity->addComponent( new Player() );

	} else if(c->name == "AIPatrol") {

		entity->addComponent( new AIPatrol(
				PARAM_FLOAT("x"),
				PARAM_FLOAT("distance")
			));

	} else if(c->name == "Position") {

		entity->addComponent( new Position(
				PARAM_FLOAT("x"),
				PARAM_FLOAT("y")
			));

	} else if(c->name == "Movement") {

		entity->addComponent( new Movement(
				PARAM_FLOAT("x"),
				PARAM_FLOAT("y")
		));

	} else if(c->name == "SpeedNeutralizer") {

		entity->addComponent( new SpeedNeutralizer(
				PARAM_FLOAT("x"),
				PARAM_FLOAT("y")
		));

	} else if(c->name == "SpeedConstraints") {

		entity->addComponent( new SpeedConstraints(
				PARAM_FLOAT("x"),
				PARAM_FLOAT("y")
		));

	} else if(c->name == "Animations") {

		entity->addComponent( new Animations(
			animationSets[ PARAM_STR("set") ],
			PARAM_STR("current")
		));

	} else if(c->name == "Sprite") {

		entity->addComponent( new Sprite(
				PARAM_STR("filename"),
				PARAM_INT("width"),
				PARAM_INT("height"),
				PARAM_INT("offsetx"),
				PARAM_INT("offsety"),
				PARAM_INT("spacingx"),
				PARAM_INT("spacingy")
		));

	} else if(c->name == "BoundingBox") {

		entity->addComponent( new BoundingBox(
				PARAM_FLOAT("left"),
				PARAM_FLOAT("top"),
				PARAM_FLOAT("width"),
				PARAM_FLOAT("height")
		));

	} else if(c->name == "KeyboardInput") {

		entity->addComponent( (KeyboardInput*)(new KeyboardInput())
				->setKey("up", SDL_SCANCODE_UP)
				->setKey("down", SDL_SCANCODE_DOWN)
				->setKey("left", SDL_SCANCODE_LEFT)
				->setKey("right", SDL_SCANCODE_RIGHT)
				->setKey("jump", SDL_SCANCODE_SPACE)
				->setKey("shoot", SDL_SCANCODE_SPACE)
			);

	}

}


Entity* EntityFactory::createTile(float x, float y, float z, int tile) {
	Entity *gameObject = new Entity();

	return gameObject;
}
