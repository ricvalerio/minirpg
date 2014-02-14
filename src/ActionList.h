/*
 * Action.h
 *
 *  Created on: 8 de Jan de 2014
 *      Author: Ricardo
 */

#ifndef ACTION_H_
#define ACTION_H_

#include <list>
#include <SDL2/SDL.h>
#include <algorithm>
#include "Entity.h"
#include "Components.h"

class TimedAction;

class ActionList {
private:
	float duration;
	float elapsed;
	std::list<TimedAction *> actions; // can be a vector or linked list

public:
	void update( float dt );

	void push_front( TimedAction *action ) {
		actions.push_front( action );
	}

	void push_back( TimedAction *action ) {
		actions.push_back( action );
	}

	void insert_before( TimedAction *elem, TimedAction *action ) {
		std::list<TimedAction *>::iterator iter = std::find( actions.begin(), actions.end(), elem );

		if(iter != actions.end())
			actions.insert(iter, action);
	}

	void insert_after( TimedAction *elem, TimedAction *action ) {
		std::list<TimedAction *>::iterator iter = std::find( actions.begin(), actions.end(), elem );

		if(iter != actions.end())
			actions.insert(++iter, action);
	}

	TimedAction *remove( TimedAction *action ) {
		std::list<TimedAction *>::iterator iter = std::find( actions.begin(), actions.end(), action );

		if(iter != actions.end())
			actions.erase( iter++ );

		return NULL;
	}


	TimedAction *begin() {
		return actions.front();
	}


	TimedAction *end() {
		return actions.back();
	}


	bool isEmpty() {
		return actions.empty();
	}
};


class TimedAction {
protected:
	ActionList *owner;

public:
	int elapsed, duration;
	bool started;
	bool finished;
	bool blocking;
	std::string name;

	TimedAction(ActionList *, int);
	virtual ~TimedAction();

	virtual void start();
	virtual void update(float);
	virtual void end();
	virtual bool isFinished() { return finished; }
	virtual bool isBlocking() { return blocking; };
};


class SyncAction : public TimedAction {
public:
	using TimedAction::start;
	using TimedAction::end;
	using TimedAction::isFinished;
	using TimedAction::isBlocking;

	SyncAction(ActionList *owner) : TimedAction(owner, 0) {
		blocking = true;
		name = " sync";

	}

	void update( float dt ) {
		if(owner->begin() == this)
			finished = true;

	}
};


class DelayAction : public TimedAction {
public:
	using TimedAction::start;
	using TimedAction::end;
	using TimedAction::isFinished;
	using TimedAction::isBlocking;

	DelayAction(ActionList *owner, int _duration) : TimedAction(owner, _duration) {
		blocking = true;
		name = "delay";
	}

	void update( float dt ) {
		elapsed += 1;

		if(elapsed > duration)
			finished = true;

	}
};

class CutSceneOutAction : public TimedAction {
public:
	using TimedAction::start;
	using TimedAction::end;
	using TimedAction::isFinished;
	using TimedAction::isBlocking;

	CutSceneOutAction(ActionList *, int);
	void update(float);
};


class CutSceneInAction : public TimedAction {
public:
	using TimedAction::start;
	using TimedAction::end;
	using TimedAction::isFinished;
	using TimedAction::isBlocking;

	CutSceneInAction(ActionList *, int);

	void update(float);
};



class FadeOutAction : public TimedAction {
public:
	using TimedAction::start;
	using TimedAction::end;
	using TimedAction::isFinished;
	using TimedAction::isBlocking;

	FadeOutAction(ActionList *, int);
	void update(float);
};


class FadeInAction : public TimedAction {
public:
	using TimedAction::start;
	using TimedAction::end;
	using TimedAction::isFinished;
	using TimedAction::isBlocking;

	FadeInAction(ActionList *, int);

	void update(float);
};


class TeleportAction : public TimedAction {
public:
	using TimedAction::start;
	using TimedAction::end;
	using TimedAction::isFinished;
	using TimedAction::isBlocking;

	Entity *entity;
	Position *position;
	float x, y;

	TeleportAction(ActionList *, Entity *, int, int);
	void update(float);
};



class MoveToAction : public TimedAction {
public:
	using TimedAction::start;
	using TimedAction::end;
	using TimedAction::isFinished;
	using TimedAction::isBlocking;

	Entity *entity;
	Movement *movement;
	Position *position;
	Animations *animations;
	float x, y;

	MoveToAction(ActionList *, int, Entity *, int, int);
	void update(float);
};


class FreezePlayerAction : public TimedAction {
public:
	using TimedAction::start;
	using TimedAction::end;
	using TimedAction::isFinished;
	using TimedAction::isBlocking;

	Entity *player;

	FreezePlayerAction(ActionList *, Entity *);
	void update(float);
};



class UnFreezePlayerAction : public TimedAction {
public:
	using TimedAction::start;
	using TimedAction::end;
	using TimedAction::isFinished;
	using TimedAction::isBlocking;

	Entity *player;

	UnFreezePlayerAction(ActionList *, Entity *);
	void update(float);
};


class SpeechAction : public TimedAction {
public:
	using TimedAction::end;
	using TimedAction::isFinished;
	using TimedAction::isBlocking;

	std::string text1, text2;
	Entity *entity;

	SpeechAction(ActionList *, int, Entity *, std::string, std::string = "");
	void start();
	void update(float);
	void end();
};


class MusicAction : public TimedAction {
public:
	using TimedAction::end;
	using TimedAction::isFinished;
	using TimedAction::isBlocking;

	std::string filename;

	MusicAction(ActionList *, int, std::string);
	void start();
	void update(float);
	void end();
};


class StopMusicAction : public TimedAction {
public:
	using TimedAction::end;
	using TimedAction::isFinished;
	using TimedAction::isBlocking;

	StopMusicAction(ActionList *, int);
	void start();
	void update(float);
	void end();
};



#endif /* ACTION_H_ */
