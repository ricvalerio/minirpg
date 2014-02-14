/*
 * AssetManager.h
 *
 *  Created on: 11 de Jan de 2014
 *      Author: Ricardo
 */

#ifndef ASSETMANAGER_H_
#define ASSETMANAGER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <map>

class TextureInfo {
public:
	std::string filename;
	SDL_Texture *texture;
	int width, height;
};



class AssetManager {
public:
	static std::map<std::string, TTF_Font *> fonts;
	static std::map<std::string, TextureInfo *> textSprites; // string is text string
	static std::map<std::string, TextureInfo *> textures; // string is filename
	static SDL_Renderer *renderer;

	AssetManager();
	virtual ~AssetManager();

	static void loadFont(std::string name, std::string file, int size, int style, int kerning);
	static TextureInfo *createText(std::string font, std::string text, SDL_Color color);
	static TextureInfo *getImageTexture(std::string filename);
	static void loadImageTexture(std::string filename);
};

#endif /* ASSETMANAGER_H_ */
