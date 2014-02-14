/*
 * GameScene.cpp
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#include <SDL2/SDL.h>
#include <typeinfo>
#include <vector>

#include "GameScene.h"
#include "SystemManager.h"
#include "Entity.h"
#include "Components.h"
#include "MovementSystem.h"
#include "GravitySystem.h"
#include "AnimationSystem.h"
#include "RenderSystem.h"
#include "TextRenderSystem.h"
#include "SpeechRenderSystem.h"
#include "HudRenderSystem.h"
#include "BoundingBoxRenderSystem.h"
#include "KeyboardInputSystem.h"
#include "CollisionSystem.h"
#include "SpeedConstraintsSystem.h"
#include "PerformActionsSystem.h"
#include "EntityFactory.h"
#include "ScriptSystem.h"

#include "AssetManager.h"
#include "RenderHelper.h"
#include "SoundManager.h"

GameScene::GameScene(SDL_Window* _window) {
	window = _window;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	AssetManager::renderer = renderer;
	RenderHelper::renderer = renderer;

	SoundManager::start();

	AssetManager::loadFont("nameplates", "arial.ttf", 20, TTF_STYLE_BOLD, 1);
	AssetManager::loadFont("text", "arial.ttf", 18, TTF_STYLE_BOLD, 1);

	gameObjects = new std::vector<Entity*>;

	systemManager = new SystemManager(gameObjects);

	Entity::_instanceid = 10000;
	Entity::_systemManager = systemManager;

	systemManager->registerSystem( new ScriptSystem(gameObjects) );
	systemManager->registerSystem( new KeyboardInputSystem(gameObjects) );
	systemManager->registerSystem( new PerformActionsSystem(gameObjects) );
	systemManager->registerSystem( new CollisionSystem(gameObjects) );
	systemManager->registerSystem( new GravitySystem(gameObjects) );
	systemManager->registerSystem( new MovementSystem(gameObjects) );
	systemManager->registerSystem( new SpeedConstraintsSystem(gameObjects) );
	systemManager->registerSystem( new AnimationSystem(gameObjects) );
	systemManager->registerSystem( new RenderSystem(gameObjects, renderer) );
	//systemManager->registerSystem( new BoundingBoxRenderSystem(gameObjects, renderer) );
	systemManager->registerSystem( new TextRenderSystem(gameObjects, renderer) );
	systemManager->registerSystem( new SpeechRenderSystem(gameObjects, renderer) );
	systemManager->registerSystem( new HudRenderSystem(gameObjects, renderer) );
	systemManager->start();

	Uint32 last_tick, cur_tick, delay,
			timeout = (Uint32) 1000 / 30;

	last_tick = SDL_GetTicks();

	running = true;
	while(running) {
		cur_tick = SDL_GetTicks();

		delay = cur_tick - last_tick;

		if(delay < timeout)
			SDL_Delay( timeout - delay );

		SDL_RenderClear(renderer);
		systemManager->update(cur_tick - last_tick);
		SDL_RenderPresent(renderer);

		last_tick = cur_tick;
	}

	systemManager->end();

    SDL_DestroyRenderer(renderer);
}


GameScene::~GameScene() {

}

