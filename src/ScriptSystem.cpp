/*
 * ScriptSystem.cpp
 *
 *  Created on: 5 de Jan de 2013
 *      Author: Ricardo
 */

#include <sstream>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstring>
#include <tgmath.h>

#include "SoundManager.h"
#include "EntityFactory.h"
#include "Components.h"
#include "Entity.h"
#include "ScriptSystem.h"
#include "ActionList.h"
#include "Helper.h"
#include "EntityWrapper.h"
#include "tinyxml2.h"


extern "C" {
	#include <lua/lua.h>
	#include <lua/lualib.h>
	#include <lua/lauxlib.h>
}

#include <iostream>
#include <luabind/luabind.hpp>


lua_State *lua_state_copy;;

std::vector<Entity *>* _entities;
std::map<std::string, std::vector<Entity *>*> subs;
std::vector<Entity *> mapObjects;

ActionList actionlist;



EntityWrapper *script_create_entity(const char *s) {
	std::string name(s, s+strlen(s));

	Entity *entity = EntityFactory::createFromType(name, NULL);
	entity->registerMe();

	return (EntityWrapper *) entity;
}


void script_playmusic(int duration, const char *file) {
	std::string str(file);

	SoundManager::loadMusic(str);
	actionlist.push_back( new MusicAction( &actionlist, duration, str) );
}


void script_stopmusic(int duration) {
	actionlist.push_back( new StopMusicAction( &actionlist, duration) );
}


void script_teleport(int id, float x, float y) {
	Entity *entity = id == 0? getPlayer(_entities) : getByID(_entities, id);

	actionlist.push_back( new TeleportAction( &actionlist, entity, x, y) );
}


void script_move_to(int duration, int id, float x, float y) {
	Entity *entity = id == 0? getPlayer(_entities) : getByID(_entities, id);

	actionlist.push_back( new MoveToAction( &actionlist, duration, entity, x, y) );
}



void script_cutscene_start(int duration) {
	actionlist.push_back( new CutSceneInAction( &actionlist, duration) );
}


void script_cutscene_end(int duration) {
	actionlist.push_back( new CutSceneOutAction( &actionlist, duration) );
}


void script_freeze_player() {
	actionlist.push_back( new FreezePlayerAction( &actionlist, getPlayer(_entities)) );
}


void script_wait(int duration) {
	actionlist.push_back( new SyncAction( &actionlist) );
	actionlist.push_back( new DelayAction( &actionlist, duration) );
	actionlist.push_back( new SyncAction( &actionlist) );
}




void script_delay(int duration) {
	actionlist.push_back( new DelayAction( &actionlist, duration) );
}


void script_sync() {
	actionlist.push_back( new SyncAction( &actionlist) );
}


void script_unfreeze_player() {
	actionlist.push_back( new UnFreezePlayerAction( &actionlist, getPlayer(_entities)) );
}


void script_subscribe(const char *s, Entity *e, luabind::object fn) {
	std::string type(s);

	luabind::call_function<void>(lua_state_copy, "add_callback", type, e->id, fn);

	subs[ type ]->push_back( e );
}


float distance(Entity *a, Entity *b) {
	Position *p1 = a->getComponent<Position>();
	Position *p2 = b->getComponent<Position>();

	float x = p1->x - p2->x;
	float y = p1->y - p2->y;

	return sqrt( x*x + y*y );
}


EntityWrapper *script_create_follow_camera(EntityWrapper *target) {
	Entity *camera = new Entity();
	camera->addComponent( new FollowCamera( (Entity *) target ) );
	camera->addComponent( new ActiveCamera() );
	camera->registerMe();
	return (EntityWrapper *) camera;
}

void script_load_metadata(const char *file) {
	EntityFactory::loadMetaData(file);
}

void script_load_animations(const char *file) {
	EntityFactory::loadAnimations(file);
}

void script_load_map(const char *mapfile) {
	if(mapObjects.size() > 0) {
		for(std::vector<Entity *>::iterator iter = mapObjects.begin(); iter != mapObjects.end(); iter++) {
			Entity *obj = *iter;
			_entities->erase(std::remove( _entities->begin(), _entities->end(), obj), _entities->end());
		}

		mapObjects.clear();
	}


	Entity *gameObject;

	tinyxml2::XMLDocument doc;
	doc.LoadFile( mapfile );

	int width, height, twidth, theight, sx, sy, ox, oy;
	int firstgid;


	tinyxml2::XMLElement *mapElement = doc.FirstChildElement("map");
	mapElement->QueryIntAttribute( "width", &width );
	mapElement->QueryIntAttribute( "height", &height );
	mapElement->QueryIntAttribute( "tilewidth", &twidth );
	mapElement->QueryIntAttribute( "tileheight", &theight );


	tinyxml2::XMLElement *tileset = mapElement->FirstChildElement("tileset");
	tileset->QueryIntAttribute( "firstgid", &firstgid );
	tileset->QueryIntAttribute( "spacing", &sx );
	tileset->QueryIntAttribute( "margin", &ox );
	sx = 0;
	ox = 0;
	sy = sx;
	oy = ox;
	int gid, tilecounter;
	float z = 0.0f;

	for(tinyxml2::XMLElement *layerElement = mapElement->FirstChildElement("layer"); layerElement != NULL; layerElement = layerElement->NextSiblingElement("layer")) {
		z += 1.0f;
		tinyxml2::XMLElement *layerDataElement = layerElement->FirstChildElement("data");

		tilecounter = 0;

		for(tinyxml2::XMLElement *tile = layerDataElement->FirstChildElement("tile"); tile != NULL; tile = tile->NextSiblingElement("tile"), tilecounter++) {
			tile->QueryIntAttribute("gid", &gid);

			if(gid == 0)
				continue;

			float tilex = ((float) twidth * (tilecounter%width));
			float tiley = ((float) theight * (tilecounter/width));

			gameObject = new Entity();
			gameObject->addComponent( new Position( tilex, tiley, z) );
			gameObject->addComponent( new Sprite("tileset4.png", (float) twidth, (float) theight, sx, sy, ox, oy, gid-1) );

			if(z == 2.0f) {
				gameObject->addComponent( new BoundingBox((float) -twidth/2, (float) -theight, (float) twidth, (float) theight) );
				gameObject->addComponent( new StaticObject() );
			}

			gameObject->registerMe();
			mapObjects.push_back(gameObject);
		}
	}
}


bool is_subscribed(Entity *entity, std::string sub) {
	if( !subs[sub] )
		return false;

	std::vector<Entity *> *vec = subs[sub];

	if( std::find( vec->begin(), vec->end(), entity) != vec->end())
		return true;

	return false;
}



ScriptSystem::ScriptSystem(std::vector<Entity *>* _gameObjects) : System(_gameObjects) {
	_entities = _gameObjects;
	lua_state = NULL;
	lua_thread = NULL;
}

ScriptSystem::~ScriptSystem() {

}


void ScriptSystem::start() {
	lua_state = luaL_newstate();

	luaL_openlibs(lua_state);

	lua_thread = lua_state; //lua_newthread(lua_state);
	lua_state_copy = lua_state;

	static const luaL_Reg lualibs[] =
	{
		{ "base", luaopen_base },
		{ NULL, NULL}
	};

	const luaL_Reg *lib = lualibs;
	for(; lib->func != NULL; lib++)
	{
		lib->func(lua_state);
		lua_settop(lua_state, 0);
	}


    using namespace luabind;
		open( lua_thread );
		module( lua_thread )
			[
			    def("create_entity", &script_create_entity),
			    def("play_music", &script_playmusic),
			    def("stop_music", &script_stopmusic),
			    def("teleport", &script_teleport),
			    def("freeze_player", &script_freeze_player),
			    def("unfreeze_player", &script_unfreeze_player),
			    def("cutscene_start", &script_cutscene_start),
			    def("cutscene_end", &script_cutscene_end),
			    def("move_to", &script_move_to),
			    def("sync", &script_sync),
			    def("subscribe", &script_subscribe),
			    def("delay", &script_delay),
			    def("wait", &script_wait),
			    def("create_follow_camera", &script_create_follow_camera),
			    def("load_metadata", &script_load_metadata),
			    def("load_animations", &script_load_animations),
			    def("load_map", &script_load_map),

			 	class_<Entity,EntityWrapper>("Entity")
			 		.def(constructor<>())
					.def_readonly("id", &EntityWrapper::id)
			 		.def("is_player", &EntityWrapper::isPlayer)
			 		.def("is_container", &EntityWrapper::isContainer)
			 		.def("is_spell", &EntityWrapper::isSpell)
			 		.def("is_attackable", &EntityWrapper::isAttackable)
			 		.def("set_name", &EntityWrapper::setName)
			 		.def("get_name", &EntityWrapper::getName)
			 		.def("set_position", &EntityWrapper::setPosition)
			 		.def("teleport", &EntityWrapper::teleport)
			 		.def("speak", (void (EntityWrapper::*)(int, const char*)) &EntityWrapper::speak)
			 		.def("speak", (void (EntityWrapper::*)(int, const char*, const char*)) &EntityWrapper::speak)
			 		.def("take_damage", &EntityWrapper::take_damage)
			 		.def("apply_force", (void (EntityWrapper::*)(Facing *, float)) &EntityWrapper::applyForce)
			 		.def("apply_force", (void (EntityWrapper::*)(float, float, float)) &EntityWrapper::applyForce)
			 		.def("has_item", &EntityWrapper::hasItem)
			 		.def("remove_item", &EntityWrapper::removeItem)
			 		.def("add_to_inventory", &EntityWrapper::addToInventory)
			 		.def("add_item", &EntityWrapper::addItem)
			 		.def("unsubscribe_all", &EntityWrapper::unsubscribeAll)
			 		.def("destroy", &EntityWrapper::destroy)
					.def("position", &EntityWrapper::getComponent<Position>)
					.def("health", &EntityWrapper::getComponent<Health>)
					.def("damage", &EntityWrapper::getComponent<Damage>)
					.def("shoot", &EntityWrapper::getComponent<ActionShoot>)
					.def("facing", &EntityWrapper::getComponent<Facing>),
				class_<Position>("Position")
					.def_readwrite("x", &Position::x)
					.def_readwrite("y", &Position::y)
					.def_readwrite("z", &Position::z)
					.def_readwrite("depth", &Position::depth),
				class_<Health>("Health")
					.def_readonly("current", &Health::amount)
					.def("takeDamage", &Health::takeDamage),
				class_<ActionShoot>("ActionShoot")
					.def("set_reload", &ActionShoot::setReload),
				class_<Facing>("Facing"),
				class_<Damage>("Damage")
					.def_readonly("amount", &Damage::amount)
			];


	subs["range"] = new std::vector<Entity *>();
	subs["collision"] = new std::vector<Entity *>();
	subs["damage"] = new std::vector<Entity *>();
	subs["action"] = new std::vector<Entity *>();
	subs["update"] = new std::vector<Entity *>();

	addScript("assets/scripts/main.lua", true);
}


void ScriptSystem::end() {
	lua_close(lua_thread);
}


void ScriptSystem::addScript(const char *file, bool run_script) {
	int script;

	if(run_script)
		script = luaL_dofile( lua_thread, file );
	else
		script = luaL_loadfile( lua_thread, file );

	if (script != 0) {
	    std::cerr << "Something went wrong loading the chunk (syntax error?)" << std::endl;
	    std::cerr << lua_tostring(lua_thread, -1) << std::endl;
	    lua_pop(lua_thread,1);
	} else {
		SDL_Log("Loaded %s", file);
	}

}

void ScriptSystem::registerEntity(Entity *entity) {
}


void ScriptSystem::update(Uint32 dt) {

	actionlist.update(dt);

	Entity *player = getPlayer(_entities);


	for(std::vector<Entity*>::iterator iter = subs["collision"]->begin(); iter != subs["collision"]->end(); ++iter) {
		Entity *entity = *iter;

		BoundingBox *box = entity->getComponent<BoundingBox>();
		if(!box)
			continue;

		std::vector<Entity *>* objects = box->getCollisions();

		for(std::vector<Entity *>::iterator iter_col = objects->begin(); iter_col != objects->end(); ++iter_col) {
			Entity *target = *iter_col;

			events.push( new CollisionEvent( entity, target) );
		}
	}


	for(std::vector<Entity*>::iterator iter = subs["range"]->begin(); iter != subs["range"]->end(); ++iter) {
		Entity *entity = *iter;

		float f = distance( player, entity );
		if( f < 200.0f ) {
			events.push( new RangeEvent( entity, player, f) );
		}
	}


	for(std::vector<Entity*>::iterator iter = subs["update"]->begin(); iter != subs["update"]->end(); ++iter) {
		Entity *entity = *iter;

		events.push( new UpdateEvent( entity ) );
	}


	while( events.size() > 0 ) {
		Event *event = events.front();
		events.pop();

		if(!event)
			break;

		if( event->type == "range") {
			RangeEvent *ev = (RangeEvent *) event;

			luabind::object o1( lua_thread, (EntityWrapper *) ev->a );
			luabind::object o2( lua_thread, (EntityWrapper *) ev->b );

			luabind::call_function<void>(lua_thread, "range_callback", o1, o2, ev->distance);

			continue;

		} else if( event->type == "collision") {

			CollisionEvent *ev = (CollisionEvent *) event;

			luabind::object o1( lua_thread, (EntityWrapper *) ev->a );
			luabind::object o2( lua_thread, (EntityWrapper *) ev->b );

			luabind::call_function<void>(lua_thread, "collision_callback", o1, o2);

			continue;

		} else if( event->type == "action") {

			ActionKeyEvent *ev = (ActionKeyEvent *) event;

			luabind::object o1( lua_thread, (EntityWrapper *) ev->a );

			luabind::call_function<void>(lua_thread, "action_callback", o1);

			continue;
		} else if( event->type == "update") {

			ActionKeyEvent *ev = (ActionKeyEvent *) event;

			luabind::object o1( lua_thread, (EntityWrapper *) ev->a );

			luabind::call_function<void>(lua_thread, "update_callback", o1);

			continue;
		}
	}


	//luabind::call_function<void>(lua_thread, "lua_update");
}

