#include "../include/fish_game/AssetManager.hpp"
// #include "../include/fish_game/ECS/Components.hpp"

#include "../include/fish_game/ECS/ProjectileComponent.hpp"
#include "../include/fish_game/ECS/SpriteComponent.hpp"
#include "../include/fish_game/ECS/TransformComponent.hpp"
#include "../include/fish_game/Game.hpp"

// #include "../include/fish_game/ECS/ProjectileComponent.hpp"

namespace FishEngine {

AssetManager::AssetManager(Manager *m) : manager(m) {}

AssetManager::~AssetManager() {}

void AssetManager::createProjectile(Vector2D pos, Vector2D vel, int range,
                                    int speed, fs::path path) {
  auto &projectile(manager->addEntity());
  projectile.addComponent<TransformComponent>(pos.getX(), pos.getY(), 16, 16,
                                              1);
  projectile.addComponent<SpriteComponent>(path);
  projectile.addComponent<ProjectileComponent>(range, speed, vel);
  projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::addTexture(std::string id, fs::path path) {
  std::cout << "Loading texture from: " << path << std::endl;
  textures[id] = TextureManager::loadTexture(path);
}

void AssetManager::addFont(std::string id, fs::path path, int fontSize) {
  // fonts[id] = TTF_OpenFont(path.c_str(), fontSize);
}

SDL_Texture *AssetManager::getTexture(std::string id) { return textures[id]; }

// TTF_Font *AssetManager::getFont(std::string id) { return fonts[id]; }

} // namespace FishEngine