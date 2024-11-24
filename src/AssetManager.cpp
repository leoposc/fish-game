#include "../include/fish_game/AssetManager.hpp"
#include "../include/fish_game/ClientGame.hpp"
#include "../include/fish_game/ECS/ProjectileComponent.hpp"
#include "../include/fish_game/ECS/SpriteComponent.hpp"
#include "../include/fish_game/ECS/TransformComponent.hpp"
#include "spdlog/spdlog.h"

namespace FishEngine {

AssetManager::AssetManager(Manager *m) : manager(m) {}

AssetManager::~AssetManager() {}

void AssetManager::addTexture(std::string id, fs::path path) {
	spdlog::get("console")->debug("Loading texture from: {}", path.string());
	textures[id] = TextureManager::loadTexture(path);
}

SDL_Texture *AssetManager::getTexture(std::string id) {
	return textures[id];
}

} // namespace FishEngine