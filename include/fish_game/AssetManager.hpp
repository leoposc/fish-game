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

  // Game objects

  void createProjectile(Vector2D pos, Vector2D vel, int range, int speed,
                        fs::path path);

  // Texture management

  void addTexture(std::string id, fs::path path);

  SDL_Texture *getTexture(std::string id);

  void addFont(std::string id, fs::path path, int fontSize);

private:
  Manager *manager;

  std::map<std::string, SDL_Texture *> textures;
  // std::map<std::string, TTF_Font *> fonts;
};

} // namespace FishEngine