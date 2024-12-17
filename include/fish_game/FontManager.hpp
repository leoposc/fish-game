#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

namespace FishEngine {

class FontManager {
  public:
	FontManager(SDL_Renderer *r, const char *fileName, uint8_t fontsize = 22);

	~FontManager();

	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL,
	            SDL_RendererFlip flip = SDL_FLIP_NONE);

	int getWidth() { return mWidth; }

	int getHeight() { return mHeight; }

  private:
	TTF_Font *gFont = nullptr;

	SDL_Texture *mTexture = nullptr;

	SDL_Renderer *renderer;

	int mWidth;
	int mHeight;
	uint8_t fontSize;
};

} // namespace FishEngine