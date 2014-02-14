/*
 * CollisionSystem.cpp
 *
 *  Created on: 20 de Dez de 2013
 *      Author: Ricardo
 */

#include <SDL2/SDL.h>
#include <cmath>
#include <limits>
#include "Helper.h"
#include "Components.h"
#include "CollisionSystem.h"
#include "System.h"



void GetSweptBroadphaseBox(BoundingBox *bp, BoundingBox* b, Movement* v) {
    bp->x =(v->vx > 0.0f ? b->x : b->x + v->vx);
    bp->y = (v->vy > 0.0f ? b->y : b->y + v->vy);
    bp->w = (v->vx > 0.0f ? v->vx + b->w : b->w - v->vx);
	bp->h = (v->vy > 0.0f ? v->vy + b->h : b->h - v->vy);
}



bool AABBCheck(BoundingBox* b1, BoundingBox* b2) {
    return !(b1->x + b1->w <= b2->x || b1->x >= b2->x + b2->w || b1->y + b1->h < b2->y || b1->y > b2->y + b2->h);
}


float SweptAABB(BoundingBox* b1, BoundingBox* b2, Movement* v1, float *normalx, float *normaly) {
	float xInvEntry, yInvEntry;
	float xInvExit, yInvExit;

	// find the distance between the objects on the near and far sides for both x and y
	if (v1->vx > 0.0f) {
		xInvEntry = b2->x - (b1->x + b1->w);
		xInvExit = (b2->x + b2->w) - b1->x;
	} else {
		xInvEntry = (b2->x + b2->w) - b1->x;
		xInvExit = b2->x - (b1->x + b1->w);
	}

	if (v1->vy > 0.0f) {
		yInvEntry = b2->y - (b1->y + b1->h);
		yInvExit = (b2->y + b2->h) - b1->y;
	} else {
		yInvEntry = (b2->y + b2->h) - b1->y;
		yInvExit = b2->y - (b1->y + b1->h);
	}




	// find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
	float xEntry, yEntry;
	float xExit, yExit;

	if (v1->vx == 0.0f) {
		xEntry = -std::numeric_limits<float>::infinity();
		xExit = std::numeric_limits<float>::infinity();
	} else {
		xEntry = xInvEntry / (float) v1->vx;
		xExit = xInvExit / (float) v1->vx;
	}

	if(v1->vy == 0.0f) {
		yEntry = -std::numeric_limits<float>::infinity();
		yExit = std::numeric_limits<float>::infinity();
	} else {
		yEntry = yInvEntry / (float) v1->vy;
		yExit = yInvExit / (float) v1->vy;
	}

	if (xEntry > 1.0f) yEntry = -std::numeric_limits<float>::infinity();
	if (yEntry > 1.0f) xEntry = -std::numeric_limits<float>::infinity();


	// find the earliest/latest times of collision
	float entryTime = std::max(xEntry, yEntry);
	float exitTime = std::min(xExit, yExit);

	// if there was no collision
	if (entryTime > exitTime)
		return 1.0f;

	if(xEntry < 0.0f && yEntry < 0.0f)
		return 1.0f;

	if (xEntry < 0.0f) {
	    // Check that the bounding box started overlapped or not.
	    if (b1->x + b1->w <= b2->x || b1->x >= b2->x + b2->w)
	    	return 1.0f;
	}

	if (yEntry < 0.0f) {
	    // Check that the bounding box started overlapped or not.
	    if (b1->y + b1->h <= b2->y || b1->y >= b2->y + b2->h)
	    	return 1.0f;
	}

	//if(xEntry > 1.0f || yEntry > 1.0f)
		//return 1.0f;

	// calculate normal of collided surface
	if (xEntry > yEntry) {
		*normaly = 0.0f;

		if (xInvEntry < 0.0f)
			*normalx = 1.0f;
		else
			*normalx = -1.0f;
	} else {
		*normalx = 0.0f;

		if (yInvEntry < 0.0f)
			*normaly = 1.0f;
		else
			*normaly = -1.0f;
	}

	// return the time of collision
	return entryTime;
}



CollisionSystem::CollisionSystem(std::vector<Entity*>* _gameObjects) : System(_gameObjects) {
	// TODO Auto-generated constructor stub
}


CollisionSystem::~CollisionSystem() {
	// TODO Auto-generated destructor stub
}


void CollisionSystem::update(Uint32 dt) {
	Position *position1, *position2;
	BoundingBox* box1, *box2;
	Movement* movement1;
	float normalx, normaly;

	float lowestCT, lowestCTnormalx, lowestCTnormaly;
	BoundingBox broadphasebox, b1, b2;
	Entity *lowestCTentity;

	for(std::vector<Entity*>::iterator iter = gameObjects->begin(); iter != gameObjects->end(); ++iter) {
		box1 = (*iter)->getComponent<BoundingBox>();

		if(box1)
			box1->resetCollisions();
	}


	// solids collision
	for(std::vector<Entity*>::iterator entity1 = gameObjects->begin(); entity1 != gameObjects->end(); ++entity1) {
		if((*entity1)->getComponent<StaticObject>() != NULL)
			continue;

		box1 = (*entity1)->getComponent<BoundingBox>();
		movement1 = (*entity1)->getComponent<Movement>();
		position1 = (*entity1)->getComponent<Position>();

		if(box1 == NULL || movement1 == NULL || position1 == NULL)
			continue;

		lowestCTentity = NULL;
		lowestCTnormalx = 0.0f;
		lowestCTnormaly = 0.0f;
		lowestCT = std::numeric_limits<float>::infinity();

		for(std::vector<Entity*>::iterator entity2 = gameObjects->begin(); entity2 != gameObjects->end(); ++entity2) {
			if(entity1 == entity2 || (*entity2)->getComponent<StaticObject>() == NULL)
				continue;

			box2 = (*entity2)->getComponent<BoundingBox>();
			//movement2 = (*entity2)->getComponent<Movement>();
			position2 = (*entity2)->getComponent<Position>();

			if(box2 == NULL || position2 == NULL)
				continue;

			normalx = 0.0f;
			normaly = 0.0f;

			b1.x = position1->x + box1->x;
			b1.y = position1->y + box1->y;
			b1.w = box1->w;
			b1.h = box1->h;

			b2.x = position2->x + box2->x;
			b2.y = position2->y + box2->y;
			b2.w = box2->w;
			b2.h = box2->h;

			GetSweptBroadphaseBox(&broadphasebox, &b1, movement1);

			if (!AABBCheck(&broadphasebox, &b2))
				continue;

			float collisiontime = SweptAABB(&b1, &b2, movement1, &normalx, &normaly);
			if(collisiontime >= 1.0f)
				continue;

			if(collisiontime > lowestCT)
				continue;

			lowestCTentity = (*entity2);
			lowestCT = collisiontime;
			lowestCTnormalx = normalx;
			lowestCTnormaly = normaly;
		}

		if(lowestCT >= 1.0f)
			continue;

		position1->x += movement1->vx * lowestCT;
		position1->y += movement1->vy * lowestCT;

		float remainingtime = 1.0f - lowestCT;

	    float dotprod = (movement1->vx * lowestCTnormaly + movement1->vy * lowestCTnormalx) * remainingtime;
	    movement1->vx = dotprod * lowestCTnormaly;
	    movement1->vy = dotprod * lowestCTnormalx;

		box1->addCollision( lowestCTentity );
		box2->addCollision( (*entity1) );

	    entity1--;
	}



	// other collisions
	for(std::vector<Entity*>::iterator entity1 = gameObjects->begin(); entity1 != gameObjects->end(); ++entity1) {
		if((*entity1)->getComponent<StaticObject>())
			continue;

		box1 = (*entity1)->getComponent<BoundingBox>();
		movement1 = (*entity1)->getComponent<Movement>();
		position1 = (*entity1)->getComponent<Position>();

		if(box1 == NULL || movement1 == NULL || position1 == NULL)
			continue;


		for(std::vector<Entity*>::iterator entity2 = gameObjects->begin(); entity2 != gameObjects->end(); ++entity2) {
			if(entity1 == entity2 || (*entity2)->getComponent<StaticObject>())
				continue;

			box2 = (*entity2)->getComponent<BoundingBox>();
			position2 = (*entity2)->getComponent<Position>();

			if(!box2 || !position2)
				continue;

			normalx = 0.0f;
			normaly = 0.0f;


			b1.x = position1->x + box1->x;
			b1.y = position1->y + box1->y;
			b1.w = box1->w;
			b1.h = box1->h;

			b2.x = position2->x + box2->x;
			b2.y = position2->y + box2->y;
			b2.w = box2->w;
			b2.h = box2->h;

			GetSweptBroadphaseBox(&broadphasebox, &b1, movement1);

			if (!AABBCheck(&broadphasebox, &b2))
				continue;

			float collisiontime = SweptAABB(&b1, &b2, movement1, &normalx, &normaly);
			if(collisiontime >= 1.0f)
				continue;

			box1->addCollision( (*entity2) );
			box2->addCollision( (*entity1) );
		}
	}
}


// deflect
/*
movement1->vx = (float) (movement1->vx) * remainingtime;
movement1->vy = (float) (movement1->vy) * remainingtime;

if (std::abs(normalx) > 0.0001f)
	movement1->vx *= -1;

if (std::abs(normaly) > 0.0001f)
	movement1->vy *= -1;
*/

// push
/*
    float magnitude = ((float) sqrt((movement1->vx * movement1->vx + movement1->vy * movement1->vy))) * remainingtime;
    float dotprod = ((float) movement1->vx) * normaly + ((float) movement1->vy) * normalx;
    if (dotprod > 0.0f)
        dotprod = 1.0f;
    else if (dotprod < 0.0f)
        dotprod = -1.0f;

    movement1->vx = dotprod * normaly * magnitude;
    movement1->vy = dotprod * normalx * magnitude;
*/
