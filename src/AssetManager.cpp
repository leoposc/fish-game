#include "../include/fish_game/AssetManager.hpp"
#include "../include/fish_game/ClientGame.hpp"
#include "../include/fish_game/ECS/ProjectileComponent.hpp"
#include "../include/fish_game/ECS/SpriteComponent.hpp"
#include "../include/fish_game/ECS/TransformComponent.hpp"
#include "../include/fish_game/FontManager.hpp"
#include "spdlog/spdlog.h"

namespace FishEngine {

AssetManager::AssetManager(Manager *m) : manager(m) {}

AssetManager::~AssetManager() {
	for (auto &tex : textures) {
		spdlog::get("console")->info("Destroying texture: {}", tex.first);
		SDL_DestroyTexture(tex.second);
	}
	for (auto &font : fonts) {
		spdlog::get("console")->info("Destroying font: {}", font.first);
		delete font.second;
	}
}

void AssetManager::addTexture(const std::string id, const fs::path path) {
	if (textures.find(id) != textures.end()) {
		spdlog::get("console")->info("Texture {} already loaded!", id);
		return;
	}

	spdlog::get("console")->debug("Loading texture from: {}", path.string());
	textures[id] = TextureManager::loadTexture(path);
}

FontManager *AssetManager::loadFont(const fs::path path, const int fontSize) {
	std::string id = path.string() + std::to_string(fontSize);
	if (fonts.find(id) != fonts.end()) {
		return fonts[id];
	}
	fonts[id] = new FontManager(ClientGame::renderer, path.c_str(), fontSize);
	return fonts[id];
}

FontManager *AssetManager::loadFromRenderedText(const std::string textureText, const fs::path path, const int fontSize,
                                                const SDL_Color textColor) {
	std::string id = path.string() + std::to_string(fontSize);
	if (fonts.find(id) == fonts.end()) {
		fonts[id] = new FontManager(ClientGame::renderer, path.c_str(), fontSize);
	}
	fonts[id]->loadFromRenderedText(textureText, textColor);
	return fonts[id];
}

void AssetManager::renderFonts() {
	for (auto &font : fonts) {
		spdlog::get("console")->info("Rendering font: {}", font.first);
		font.second->render((1280 - font.second->getWidth()) / 2, 720 / 3);
	}
}

SDL_Texture *AssetManager::getTexture(const std::string id) const {
	auto it = textures.find(id);
	if (it != textures.end()) {
		return it->second;
	}
	spdlog::get("stderr")->info("Texture {} not found!", id);
	return nullptr;
}

} // namespace FishEngine