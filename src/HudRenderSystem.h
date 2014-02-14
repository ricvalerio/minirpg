/*
 * HudRenderSystem.h
 *
 *  Created on: 2 de Jan de 2014
 *      Author: Ricardo
 */

#ifndef HUDRENDERSYSTEM_H_
#define HUDRENDERSYSTEM_H_

#include <vector>
#include <SDL2/SDL.h>
#include "Entity.h"
#include "RenderSystem.h"
#include "Components.h"
#include "System.h"
#include "AssetManager.h"
#include "RenderHelper.h"
#include "Helper.h"


class HUDWidget {
public:
	HUDWidget() {};
	virtual ~HUDWidget() {};

	virtual void start() {};
	virtual void update() {};
	virtual void draw(SDL_Renderer *) {};
};


class HUDContainer : public HUDWidget {
public:
	std::vector<Entity *>* items;
	SDL_Rect dst;
	int timer, offset;

	HUDContainer(std::vector<Entity *>* e) {
		timer = 120;
		offset = 255;

		items = e;
		dst.x = 0;
		dst.y = 0;
		dst.w = 0;
		dst.h = 0;
	};

	~HUDContainer() {};

	void start() {
		dst.w = 300 + 32 + 3*3;
		dst.h = (32 + 3) * items->size() + 15 + 12;
		dst.x = 800 - dst.w - 20;
		dst.y = 600 - dst.h - 20;

		timer = 150;
		offset = 0;
	}

	void update() {
		if(timer == 0)
			return;

		timer--;
		if(timer > 60)
			offset = 0;
		else
			offset = dst.w+20 - timer * (dst.w + 20) / 60;
	}

	void draw(SDL_Renderer *renderer) {
		if(timer == 0)
			return;

		int b = 15;

		SDL_Rect r = {dst.x + offset, dst.y, dst.w, dst.h};
		RenderHelper::drawBox(&r, 15);

		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
		SDL_Color white = {255, 255, 255 };
		int i=0;
		SDL_Rect srcRect, dstRect, irect;
		srcRect.x = 0;
		srcRect.y = 0;

		dstRect.x = dst.x + 32 + 3 + b + offset;
		dstRect.y = dst.y + b + (32+3) * i;

		irect.x = dst.x + 15 + offset;
		irect.y = dst.y + 15;
		irect.w = 32;
		irect.h = 32;

		for(std::vector<Entity *>::iterator iter = items->begin(); iter != items->end(); ++iter, i++) {
			Entity *e = *iter;
			Sprite *sprite = e->getComponent<Sprite>();
			if(!sprite) {
				i--;
				continue;
			}

			irect.y += (32+3)*i;

			SDL_RenderCopy(renderer, AssetManager::getImageTexture( sprite->filename )->texture, sprite->getFrame(), &irect);
			TextureInfo *textTexture = AssetManager::createText("nameplates", e->getComponent<Name>()->name, white);

			srcRect.w = textTexture->width;
			srcRect.h = textTexture->height;

			dstRect.w = srcRect.w;
			dstRect.h = srcRect.h;
			dstRect.y += (32+3) * i;
			SDL_RenderCopy(renderer, textTexture->texture, &srcRect, &dstRect);

		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	}
};


class HudRenderSystem : public System {
public:
	using System::start;
	using System::end;
	using System::registerEntity;
	static std::vector<HUDWidget *> widgets;

	SDL_Renderer* renderer;

	HudRenderSystem(std::vector<Entity*>* v, SDL_Renderer* r);
	virtual ~HudRenderSystem();

	static void addWidget( HUDWidget *w ) {
		widgets.push_back( w );
		w->start();
	};

	static void createContainer( std::vector<Entity *>* e ) {
		HUDContainer *c = new HUDContainer(e);
		addWidget(c);
	};

	void update(Uint32);
};

#endif /* HUDRENDERSYSTEM_H_ */
