/*
 * AssetManager.cpp
 *
 *  Created on: 11 de Jan de 2014
 *      Author: Ricardo
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "AssetManager.h"

SDL_Renderer *AssetManager::renderer = NULL;
std::map<std::string, TextureInfo*> AssetManager::textSprites;
std::map<std::string, TextureInfo*> AssetManager::textures;
std::map<std::string, TTF_Font *> AssetManager::fonts;

AssetManager::AssetManager() {
	// TODO Auto-generated constructor stub

}

AssetManager::~AssetManager() {
	// TODO Auto-generated destructor stub
}


TextureInfo *AssetManager::getImageTexture(std::string filename) {
	if(!textures[filename]) {
		loadImageTexture(filename);
	}

	return textures[filename];
}

void AssetManager::loadImageTexture(std::string filename) {
	TextureInfo *textureinfo = new TextureInfo();
	textureinfo->filename = filename;

	std::string file = "assets/sprites/"+filename;
	SDL_Surface *surface = IMG_Load( file.c_str() );
	if(surface == NULL) {
		SDL_Log("Failed to load %s", file.c_str());
		SDL_Quit();
	}

	textureinfo->texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(textureinfo->texture, NULL, NULL, &textureinfo->width, &textureinfo->height);
	SDL_FreeSurface(surface);

	textures[ filename ] = textureinfo;
}


void AssetManager::loadFont(std::string name, std::string file, int size, int style, int kerning) {
	TTF_Font *font = TTF_OpenFont(("assets/fonts/"+file).c_str(), size);
	TTF_SetFontStyle( font, style);
	TTF_SetFontKerning( font, kerning);

	fonts[ name ] = font;
}


TextureInfo *AssetManager::createText(std::string font, std::string text, SDL_Color color) {
	if( !textSprites[ text ] ) {
		TextureInfo *textureinfo = new TextureInfo();
		textureinfo->texture = SDL_CreateTextureFromSurface( renderer, TTF_RenderText_Solid( fonts[ font ], text.c_str(), color));
		SDL_QueryTexture(textureinfo->texture, NULL, NULL, &textureinfo->width, &textureinfo->height);
		textSprites[ text ] = textureinfo;
	}

	return textSprites[ text ];
}
