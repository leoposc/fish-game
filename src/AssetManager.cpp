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
		std::cout << "Destroying texture: " << tex.first << std::endl;
		SDL_DestroyTexture(tex.second);
	}
	for (auto &font : fonts) {
		std::cout << "Destroying font: " << font.first << std::endl;
		delete font.second;
	}
}

void AssetManager::addTexture(std::string id, fs::path path) {
	spdlog::get("console")->debug("Loading texture from: {}", path.string());
	textures[id] = TextureManager::loadTexture(path);
}

FontManager *AssetManager::loadFont(fs::path path, int fontSize) {
	std::string id = path.string() + std::to_string(fontSize);
	if (fonts.find(id) != fonts.end()) {
		return fonts[id];
	}
	fonts[id] = new FontManager(ClientGame::renderer, path.c_str(), fontSize);
	return fonts[id];
}

FontManager *AssetManager::loadFromRenderedText(std::string textureText, fs::path path, int fontSize,
                                                SDL_Color textColor) {
	std::string id = path.string() + std::to_string(fontSize);
	if (fonts.find(id) == fonts.end()) {
		fonts[id] = new FontManager(ClientGame::renderer, path.c_str(), fontSize);
	}
	fonts[id]->loadFromRenderedText(textureText, textColor);
	return fonts[id];
}

void AssetManager::renderFonts() {
	for (auto &font : fonts) {
		std::cout << "Rendering font: " << font.first << std::endl;
		font.second->render((1280 - font.second->getWidth()) / 2, 720 / 3);
	}
}

SDL_Texture *AssetManager::getTexture(std::string id) {
	return textures[id];
}

} // namespace FishEngine