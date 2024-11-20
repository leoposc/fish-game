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

class AssetManager {
  public:
	AssetManager(Manager *man);
	~AssetManager();

	void addTexture(std::string id, fs::path path);

	SDL_Texture *getTexture(std::string id);

  private:
	Manager *manager;

	std::map<std::string, SDL_Texture *> textures;
};

} // namespace FishEngine