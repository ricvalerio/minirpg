/*
 * RenderHelper.h
 *
 *  Created on: 11 de Jan de 2014
 *      Author: Ricardo
 */

#ifndef RENDERHELPER_H_
#define RENDERHELPER_H_

#include <SDL2/SDL.h>

class RenderHelper {
public:
	static SDL_Renderer *renderer;

	RenderHelper();
	virtual ~RenderHelper();

	static void drawBox(SDL_Rect *dstrect, int b);
};

#endif /* RENDERHELPER_H_ */
