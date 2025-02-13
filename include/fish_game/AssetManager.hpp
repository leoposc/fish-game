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

	void addTexture(const std::string id, const fs::path path);

	FontManager *loadFont(const fs::path path, const int fontSize);

	FontManager *loadFromRenderedText(const std::string textureText, const fs::path path, const int fontSize,
	                                  const SDL_Color textColor);

	void renderFonts();

	SDL_Texture *getTexture(const std::string id) const;

  private:
	Manager *manager;

	std::unordered_map<std::string, FontManager *> fonts;

	std::unordered_map<std::string, SDL_Texture *> textures;
};

} // namespace FishEngine