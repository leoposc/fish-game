#pragma once

#include "ECS/ECS.hpp"
#include "TextureManager.hpp"
#include "Vector2D.hpp"

#include <SDL2/SDL.h>
#include <filesystem>
#include <map>
#include <string>

namespace fs = std::filesystem;
namespace FishEngine {

class FontManager;

class AssetManager {
  public:
	AssetManager(Manager *man);
	~AssetManager();

	void addTexture(std::string id, fs::path path);

	FontManager *loadFont(fs::path path, int fontSize);

	FontManager *loadFromRenderedText(std::string textureText, fs::path path, int fontSize, SDL_Color textColor);

	void renderFonts();

	SDL_Texture *getTexture(std::string id);

  private:
	Manager *manager;

	std::map<std::string, FontManager *> fonts;

	std::map<std::string, SDL_Texture *> textures;
};

} // namespace FishEngine