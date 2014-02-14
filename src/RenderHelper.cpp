/*
 * RenderHelper.cpp
 *
 *  Created on: 11 de Jan de 2014
 *      Author: Ricardo
 */

#include <SDL2/SDL.h>
#include "RenderHelper.h"
#include "AssetManager.h"


SDL_Renderer *RenderHelper::renderer = NULL;


RenderHelper::RenderHelper() {
	// TODO Auto-generated constructor stub

}

RenderHelper::~RenderHelper() {
	// TODO Auto-generated destructor stub
}


void RenderHelper::drawBox(SDL_Rect *dstrect, int b) {
	TextureInfo *box = AssetManager::getImageTexture( std::string("box2.png") );

	int rw = box->width;
	int rh = box->height;
	int rb = 15;

	// top left
	SDL_Rect rect = {0, 0, rb, rb};
	SDL_Rect dstr = {dstrect->x, dstrect->y, b, b};
	SDL_RenderCopy(renderer, box->texture, &rect, &dstr);

	// top right
	rect.x = rw - rb;
	dstr.x = dstrect->x + dstrect->w - b;
	SDL_RenderCopy(renderer, box->texture, &rect, &dstr);

	// bottom right
	rect.y = rh - rb;
	dstr.y = dstrect->y + dstrect->h - b;
	SDL_RenderCopy(renderer, box->texture, &rect, &dstr);

	// bottom left
	rect.x = 0;
	dstr.x = dstrect->x;
	SDL_RenderCopy(renderer, box->texture, &rect, &dstr);

	rect.x = rb;
	rect.y = 0;
	rect.w = rw - rb*2;
	rect.h = rb;

	dstr.x = dstrect->x + b;
	dstr.y = dstrect->y;
	dstr.w = dstrect->w - b*2;
	dstr.h = b;
	SDL_RenderCopy(renderer, box->texture, &rect, &dstr);

	rect.y = rh - rb;
	dstr.y = dstrect->y + dstrect->h - b;
	SDL_RenderCopy(renderer, box->texture, &rect, &dstr);

	rect.x = 0;
	rect.y = rb;
	rect.w = rb;
	rect.h = rw - rb*2;

	dstr.x = dstrect->x;
	dstr.w = b;
	dstr.y = dstrect->y + b;
	dstr.h = dstrect->h - b*2;
	SDL_RenderCopy(renderer, box->texture, &rect, &dstr);

	rect.x = rw - rb;
	dstr.x = dstrect->x + dstrect->w - b;
	SDL_RenderCopy(renderer, box->texture, &rect, &dstr);


	rect.x = rb;
	rect.y = rb;
	rect.w = rw - rb*2;
	rect.h = rh - rb*2;
	dstr.x = dstrect->x + b;
	dstr.y = dstrect->y + b;
	dstr.w = dstrect->w - b*2;
	dstr.h = dstrect->h - b*2;

	SDL_RenderCopy(renderer, box->texture, &rect, &dstr);


}
