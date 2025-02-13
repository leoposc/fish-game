#include "../include/fish_game/FontManager.hpp"
#include "../include/fish_game/AssetManager.hpp"
#include "../include/fish_game/ClientGame.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <spdlog/spdlog.h>
#include <string>

namespace FishEngine {

FontManager::FontManager(SDL_Renderer *r, const char *fileName, uint8_t fontsize) : renderer(r), fontSize(fontsize) {
	if (TTF_Init() == -1) {
		spdlog::get("stderr")->error("SDL_ttf could not initialize! SDL_ttf Error: {}", TTF_GetError());
	}
	gFont = TTF_OpenFont(fileName, fontSize);
	if (gFont == nullptr) {
		spdlog::get("stderr")->error("Failed to load font! SDL_ttf Error: {}", TTF_GetError());
	}
}

FontManager::~FontManager() {
	TTF_CloseFont(gFont);

	if (mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

bool FontManager::loadFromRenderedText(std::string textureText, SDL_Color textColor) {

	SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);

	if (textSurface != NULL) {
		mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (mTexture == NULL) {
			spdlog::get("stderr")->error("Unable to create texture from rendered text! SDL Error:{}", SDL_GetError());
		} else {
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		// get rid of old surface
		SDL_FreeSurface(textSurface);
	} else {
		spdlog::get("stderr")->error("Unable to render text surface! SDL_ttf Error: {}", TTF_GetError());
	}

	return mTexture != NULL;
}

void FontManager::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip) {

	SDL_Rect renderQuad = {x, y, mWidth, mHeight};
	spdlog::get("fb")->debug("Rendering font, x: {}, y:{}", x, y);

	// Set clip rendering dimensions
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	spdlog::get("fb")->debug("Rendering font, x: {}, y:{}", x, y);

	SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flip);
}

} // namespace FishEngine
