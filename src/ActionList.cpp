/*
 * Action.cpp
 *
 *  Created on: 8 de Jan de 2014
 *      Author: Ricardo
 */

#include "ActionList.h"
#include "Entity.h"
#include "Components.h"
#include "RenderSystem.h"
#include "SoundManager.h"
#include "Helper.h"


void ActionList::update( float dt ) {
	std::list<TimedAction *>::iterator iter = actions.begin();

	while(iter != actions.end()) {
		TimedAction *action = *iter;

		if(!action->started) {
			action->start();
			action->started = true;
		}

		action->update( dt );

		if(action->isFinished()) {
			++iter;

			action->end();

			action = this->remove( action );
		} else {
			if(action->isBlocking())
				break;

			++iter;
		}

	}

}


TimedAction::TimedAction(ActionList *_owner, int _duration) {
	duration = _duration;
	owner = _owner;


	finished = false;
	blocking = false;
	elapsed = 0;
	started = false;
};

TimedAction::~TimedAction() {

};


void TimedAction::start() {
	started = true;
};

void TimedAction::update(float dt) {

};


void TimedAction::end() {

};



CutSceneOutAction::CutSceneOutAction(ActionList *owner, int _duration = 60) : TimedAction(owner, _duration) {
	name = "cutscene out";
}

void CutSceneOutAction::update(float dt) {
	elapsed++;

	RenderSystem::cutscene_alpha = 255 * (duration-elapsed) / duration;

	if(elapsed >= duration)
		finished = true;

}


CutSceneInAction::CutSceneInAction(ActionList *owner, int _duration = 60) : TimedAction(owner, _duration) {
	name = "cutscene in";
}

void CutSceneInAction::update(float dt) {
	elapsed++;

	RenderSystem::cutscene_alpha = 255 * elapsed / duration;

	if(elapsed >= duration)
		finished = true;

}




FadeOutAction::FadeOutAction(ActionList *owner, int _duration = 60) : TimedAction(owner, _duration) {
	name = "fade out";
}

void FadeOutAction::update(float dt) {
	elapsed++;

	RenderSystem::fade_alpha = 255 * (duration-elapsed) / duration;

	if(elapsed >= duration)
		finished = true;

}


FadeInAction::FadeInAction(ActionList *owner, int _duration = 60) : TimedAction(owner, _duration) {
	name = "fade in";
}

void FadeInAction::update(float dt) {
	elapsed++;

	RenderSystem::fade_alpha = 255 * elapsed / duration;

	if(elapsed >= duration)
		finished = true;

}



TeleportAction::TeleportAction(ActionList *owner, Entity *_e, int _x, int _y) : TimedAction(owner, 0) {
	x = _x;
	y = _y;
	name = "teleport";

	entity = _e;

	position = entity->getComponent<Position>();
}


void TeleportAction::update(float dt) {
	position->x = x;
	position->y = y;

	finished = true;
}


MoveToAction::MoveToAction(ActionList *owner, int _duration, Entity *_e, int _x, int _y) : TimedAction(owner, _duration) {
	x = _x;
	y = _y;
	name = "move to";

	entity = _e;

	movement = entity->getComponent<Movement>();
	position = entity->getComponent<Position>();
	animations = entity->getComponent<Animations>();
}

void MoveToAction::update(float dt) {

	float diffx = x - position->x;
	float diffy = y - position->y;

	if( abs(diffx) > 2) {
		if(diffx > 0) {
			Action* action = entity->getComponent<ActionWalkRight>();
			if(action)
				action->perform();
		} else if(diffx < 0) {
			Action* action = entity->getComponent<ActionWalkLeft>();
			if(action)
				action->perform();
		}
	} else {
		diffx = 0;
		movement->vx = 0;
	}


	if( abs(diffy) > 2) {
		if(diffy > 0) {
			Action* action = entity->getComponent<ActionWalkDown>();
			if(action)
				action->perform();
		} else if(diffy < 0) {
			Action* action = entity->getComponent<ActionWalkUp>();
			if(action)
				action->perform();
		}
	} else {
		diffy = 0;
		movement->vy = 0;
	}


	if(movement->vx == 0) {
		if(movement->vy > 0)
			animations->setCurrent("down");
		else if(movement->vy < 0)
			animations->setCurrent("up");
		else
			animations->setCurrent("still");
	} else if(movement->vx > 0) {
		animations->setCurrent("right");
	} else if(movement->vx < 0) {
		animations->setCurrent("left");
	}

	if(diffx == 0 && diffy == 0)
		finished = true;
}


FreezePlayerAction::FreezePlayerAction(ActionList *owner, Entity *e) : TimedAction(owner, 0) {
	player = e;
	name = "freeze";

}


void FreezePlayerAction::update(float dt) {
	player->getComponent<KeyboardInput>()->freeze = true;
	player->getComponent<Animations>()->setCurrent("still");

	finished = true;
}




UnFreezePlayerAction::UnFreezePlayerAction(ActionList *owner, Entity *e) : TimedAction(owner, 0) {
	player = e;
	name = "unfreeze";

}


void UnFreezePlayerAction::update(float dt) {
	player->getComponent<KeyboardInput>()->freeze = false;

	finished = true;
}


SpeechAction::SpeechAction(ActionList *owner, int _duration, Entity *_entity, std::string _text1, std::string _text2) : TimedAction(owner, _duration) {
	entity = _entity;
	text1 = _text1;
	text2 = _text2;
	name = "speech";
	elapsed = 0;
}

void SpeechAction::start() {
	entity->addComponent( new ChatBubble(text1, text2, -1) );
	started = true;
}

void SpeechAction::update(float dt) {
	if(duration > 0) {
		elapsed++;

		if(elapsed >= duration)
			finished = true;

	} else {

		SDL_PumpEvents();
		const Uint8 *state = SDL_GetKeyboardState(NULL);

		bool action_key = state[ SDL_SCANCODE_SPACE ];
		if( action_key )
			finished = true;
	}
}

void SpeechAction::end() {
	entity->removeComponent<ChatBubble>();
}





MusicAction::MusicAction(ActionList *owner, int _duration, std::string _file) : TimedAction(owner, _duration) {
	filename = _file;
	name = "music";
	elapsed = 0;
}

void MusicAction::start() {
	SoundManager::loadMusic(filename);
	started = true;
}

void MusicAction::update(float dt) {
	SoundManager::playMusic(filename, -1, duration);
	finished = true;
}

void MusicAction::end() {

}




StopMusicAction::StopMusicAction(ActionList *owner, int _duration) : TimedAction(owner, _duration) {
	name = "stopmusic";
	elapsed = 0;
}

void StopMusicAction::start() {
	started = true;
}

void StopMusicAction::update(float dt) {
	SoundManager::stopMusic(duration);
	finished = true;
}

void StopMusicAction::end() {

}



