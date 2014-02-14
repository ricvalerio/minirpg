/*
 * Event.h
 *
 *  Created on: 13 de Jan de 2014
 *      Author: Ricardo
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <string>
#include "Entity.h"


class Event {
public:
	std::string type;

	Event();
	virtual ~Event();
};


class ActionKeyEvent : public Event {
public:
	Entity *a;

	ActionKeyEvent(Entity *_a) {
		type = "action";

		a = _a;
	}
};


class UpdateEvent : public Event {
public:
	Entity *a;

	UpdateEvent(Entity *_a) {
		type = "update";

		a = _a;
	}
};

class RangeEvent : public Event {
public:
	float distance;
	Entity *a, *b;

	RangeEvent(Entity *_a, Entity *_b, float _d) {
		type = "range";

		a = _a;
		b = _b;
		distance = _d;
	}
};

class CollisionEvent : public Event {
public:
	Entity *a, *b;

	CollisionEvent(Entity *_a, Entity *_b) {
		type = "collision";

		a = _a;
		b = _b;
	}
};

#endif /* EVENT_H_ */
