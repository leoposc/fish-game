#include "../include/fish_game/AssetManager.hpp"
#include "../include/fish_game/ClientGame.hpp"
#include "../include/fish_game/ECS/ProjectileComponent.hpp"
#include "../include/fish_game/ECS/SpriteComponent.hpp"
#include "../include/fish_game/ECS/TransformComponent.hpp"

namespace FishEngine {

AssetManager::AssetManager(Manager *m) : manager(m) {}

AssetManager::~AssetManager() {
	for (auto &tex : textures) {
		std::cout << "Destroying texture: " << tex.first << std::endl;
		SDL_DestroyTexture(tex.second);
	}
}

void AssetManager::addTexture(std::string id, fs::path path) {
	std::cout << "Loading texture from: " << path << std::endl;
	textures[id] = TextureManager::loadTexture(path);
}

SDL_Texture *AssetManager::getTexture(std::string id) {
	return textures[id];
}

} // namespace FishEngine