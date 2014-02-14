
#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_


extern "C" {
	#include <lua/lua.h>
	#include <lua/lualib.h>
	#include <lua/lauxlib.h>
}

#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <tr1/unordered_map>
#include <typeinfo>
#include <SDL2/SDL.h>
#include "IComponent.h"

class Entity;

class Container : public IComponent {
public:
	std::vector<Entity *> container; // contains entities + number of them

	Container() {
		container.clear();
	}

	void add(Entity *e) {
		container.push_back(e);
	}

	void remove(Entity *e) {
		std::vector<Entity *>::iterator iter = std::find( container.begin(), container.end(), e );

		if(iter != container.end())
			container.erase( iter );
	}

	std::vector<Entity *> *getAll() {
		return &container;
	}
};


class Money : public IComponent {
public:
	int amount;

	Money(int _amount) {
		amount = _amount;
	}
};


class Item : public IComponent {
public:
	enum TYPES { HEALS, DAMAGES, AMMO, GOLD };
};


class Inventory : public Container {

};


class Camera : public IComponent {};
class ActiveCamera : public Camera {};
class StaticCamera : public Camera {};

class FollowCamera : public Camera {
public:
	void *target;

	FollowCamera(void *_target) {
		target = _target;
	}
};


class Damage : public IComponent {
public:
	int amount;

	Damage(int _amount) {
		amount = _amount;
	}
};


class Player : public IComponent {

};


class Pickable : public IComponent {
public:
	bool picked;

	Pickable() {
		picked = false;
	}

	bool isPicked() {
		return picked;
	}

	void pickup() {
		picked = true;
	}
};




class Script : public IComponent {
public:
	std::string filename;
	int script;

	bool in_range;
	bool interacting;

	Script(std::string _file) {
		filename = _file;

		in_range = false;
		interacting = false;

		script = -1;

	}
};


class Position : public IComponent {
public:
	float x, y, z, depth;

	Position(float _x, float _y, float _depth = 2.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
		depth = _depth;
	}
};



class HUDPosition : public Position {
public:
	HUDPosition(float _x, float _y) : Position(_x, _y) {

	};
};


class Facing : public IComponent {
public:
	bool top, down, left, right;

	Facing(bool t = false, bool d = false, bool l = false, bool r = false) {
		top = t;
		down = d;
		left = l;
		right = r;
	}

	void reset() {
		top = false;
		down = false;
		left = false;
		right = false;
	}

	void setUp() { top = true; down = false; left = false; right = false; };
	void setDown() { top = false; down = true; left = false; right = false; };
	void setLeft() { top = false; down = false; left = true; right = false; };
	void setRight() { top = false; down = false; left = false; right = true; };

	bool isFacingUp() { return top; }
	bool isFacingDown() { return down; }
	bool isFacingLeft() { return left; }
	bool isFacingRight() { return right; }
};


class Movement : public IComponent {
public:
	float vx, vy, vz;
	float ax, ay, az;

	Movement(float _vx, float _vy, float _vz=0.0f) {
		vx = _vx;
		vy = _vy;
		vz = _vz;

		ax = 0;
		ay = 0;
		az = 0;
	}
};

class SpeedNeutralizer : public IComponent {
public:
	float ax, ay;

	SpeedNeutralizer(float _ax, float _ay) {
		ax = _ax;
		ay = _ay;
	}
};

class SpeedConstraints : public IComponent {
public:
	float x, y;

	SpeedConstraints(float _x, float _y) {
		x = _x;
		y = _y;
	}
};


class Gravity : public IComponent {
public:
	float acceleration;

	Gravity(float _accel) {
		acceleration = _accel;
	}
};


class Health : public IComponent {
public:
	int amount, max_amount;

	Health(int x) {
		max_amount = x;
		amount = x;
	}

	void takeDamage(int x) { amount -= x; if(amount < 0) amount = 0; }
	void healDamage(int x) { amount += x; if(amount > max_amount) amount = max_amount; }
	void die() { amount = 0; }
	bool isDead() { return amount <= 0; }
};


class Name : public IComponent {
public:
	std::string name;

	Name(std::string _name) {
		name = _name;
	}
};


class NPC : public IComponent {

};

class Team : public IComponent {
public:
	int team_id;
};


class Hostile : public IComponent {
public:
	bool hostile;
};


class Timer : public IComponent {
public:
	int timer;

	Timer(int x) {
		timer = x;
	}
};


class ChatBubble : public IComponent {
public:
	std::string text1, text2;
	int timer;

	ChatBubble(std::string _text1, std::string _text2, int _timer = 150) {
		timer = _timer;
		text1 = _text1;
		text2 = _text2;
	}
};

class CountdownTimer : public IComponent {
public:
	int timer, max_timer;

	CountdownTimer(int x) {
		timer = x;
		max_timer = x;
	}

	bool finished() { return timer <= 0; }
	void reset() { timer = max_timer; }
};


class BoundingBox : public IComponent {
public:
	int x, y, w, h;
	std::vector<Entity *> collisions;

	BoundingBox() {
		x = 0;
		y = 0;
		w = 0;
		h = 0;
	};

	BoundingBox(int _x, int _y, int _w, int _h) {
		x = _x;
		y = _y;
		w = _w;
		h = _h;
	}

	void addCollision(Entity *e) {
		collisions.push_back(e);
	}

	std::vector<Entity *> *getCollisions() {
		return &collisions;
	}

	void resetCollisions() {
		collisions.clear();
	}
};


class StaticObject : public IComponent {
public:

};

class Sprite : public IComponent {
public:
	std::string filename;

	int offset_x, offset_y,
		spacing_x, spacing_y,
		total_width, total_height,
		width, height,
		current_frame;
	SDL_Rect rect;


	Sprite(std::string file, int w=-1, int h=-1, int ox=0, int oy=0, int sx=0, int sy=0, int frame=0) {
		filename = file;

		total_width = 0;
		total_height = 0;

		width = w;
		height = h;

		offset_x = ox;
		offset_y = oy;

		spacing_x = sx;
		spacing_y = sy;

		current_frame = frame;

		rect.w = width;
		rect.h = height;

	}


	void selectFrame(int frame) {
		int tw;

		current_frame = frame;

		tw = total_width + spacing_x;

		int n_cols = (tw-offset_x) / (width + spacing_x);

		int y = current_frame / n_cols;
		int x = current_frame % n_cols;

		rect.x = offset_x + (width+spacing_x)*x;
		rect.y = offset_y + (height+spacing_y)*y;

	}


	SDL_Rect *getFrame() {
		return &rect;
	}
};


// animation component defines only one animation, in a single sprite
class Animation : public IComponent {
public:
	std::string name;
	std::vector<int>* frames;
	int frame, fps, fps_time;

	Uint32 delta;

	Animation(std::string _name, std::vector<int>* _frames, int _fps, int _frame = 0) {
		name = _name;
		frames = _frames;
		fps = _fps;
		fps_time = 1000 / _fps;
		delta = 0;
		frame = frames->at(_frame);
	}
};



// groups several animations
class AnimationSet : public IComponent {
public:
	std::map<std::string, Animation*> animations;

	void addAnimation(std::string name, Animation* animation) {
		animations[name] = animation;
	}
};


// animations the entity will have: a animation set and the current one
class Animations : public IComponent {
public:
	AnimationSet *set;
	std::string current;
	bool running;

	Animations(AnimationSet *_set, std::string _current) {
		set = _set;
		current = _current;
		running = true;
	}

	void stop() {
		running = false;
		set->animations[current]->frame = 0;
	}

	void start() {
		running = true;
	}

	bool isRunning() {
		return running;
	}

	void setCurrent(std::string name) {
		current = name;
	}

	Animation* getCurrent() {
		return set->animations[current];
	}
};



class KeyboardInput : public IComponent {
public:
	std::map<std::string, int> binds;
	bool freeze;

	KeyboardInput* setKey(std::string name, int key) {
		binds[name] = key;
		freeze = false;
		return this;
	}

	int getKey(std::string name) {
		return binds[name];
	}
};



class Action : public IComponent {
public:
	bool active, available;

	Action() {
		active = false;
		available = true;
	};

	void perform() { active = true; };
	void stop() { active = false; };
	void setAvailable(bool flag) { available = flag; }
	bool isReady() { return available; }
};


class ActionWalkUp : public Action {};
class ActionWalkDown : public Action {};
class ActionWalkRight : public Action {};
class ActionWalkLeft : public Action {};
class ActionJump : public Action {};
class ActionShoot : public Action {
public:
	int reload_time, timer;

	ActionShoot(int r = 30) {
		reload_time = r;
		timer = 0;
	}

	void setReload(int r) {
		reload_time = r;
	}

};


class AIPatrol : public IComponent {
public:
	float x, distance;

	AIPatrol( float _x, float _distance ) {
		x = _x;
		distance = _distance;
	}
};


class Transition : public IComponent {
public:
	int current;
	int timer;

	Transition() {
		current = 0;
		timer = 120;
	}
};

class MoveToTransition : public Transition {
public:
	float start_x, start_y;
	float end_x, end_y;

	MoveToTransition(float _x1, float _y1, float _x2, float _y2) : Transition() {
		start_x = _x1;
		start_y = _y1;

		end_x = _x2;
		end_y = _y2;
	}
};


class Transitions : public IComponent {
private:
	std::tr1::unordered_map<const std::type_info*, Transition *> transitions;

public:

	template <typename T>
	void addTransition(T* transition) {
		transitions[ &typeid(T) ] = transition;
	};


	template <typename T>
	T* getTransition() {
	    if(transitions.count( &typeid(T) ) != 0) {
	        return (T*) (transitions[&typeid(T)]);
	    } else {
	        return NULL;
	    }
	};

};



#endif
