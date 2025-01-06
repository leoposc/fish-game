#include "../include/fish_game/Map.hpp"
#include "../include/fish_game/ClientGame.hpp"
#include "spdlog/spdlog.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>

namespace FishEngine {

void Map::loadMap(fs::path path, bool hasBackground) {

	if (!fs::exists(path)) {
		spdlog::get("stderr")->error("Map file does not exist!");
		return;
	}

	map = tileson.parse(path);

	positionOffset = {0, 0};
	currentMap = map.get();

	if (map->getStatus() == tson::ParseStatus::OK) {
		spdlog::get("console")->debug("Map loaded successfully!");
	} else {
		spdlog::get("stderr")->error("Map loading failed!");
		return;
	}

	if (hasBackground) {
		loadBackground();
	}
	loadTilesetTextures();
}

void Map::drawMap() {
	for (auto &layer : currentMap->getLayers()) {
		drawLayer(layer);
	}
}

fs::path Map::getTilePath(int id) {
	// fetch the path to the tileset
	// use the id to get the right tileset
	fs::path path = map->getTileset("tileset")->getImagePath();
	return path;
}

void Map::drawLayer(tson::Layer &layer) {
	switch (layer.getType()) {
	case tson::LayerType::TileLayer:
		drawTileLayer(layer);
		break;
	case tson::LayerType::ObjectGroup:
		drawObjectLayer(layer);
		break;

	case tson::LayerType::ImageLayer:
		drawImageLayer(layer);
		break;
	default:
		break;
		// spdlog::get("console")->debug( "Layer type not supported!" );
	}
}

void Map::drawTileLayer(tson::Layer &layer, int offset_y) {
	float tileHeight = layer.getMap()->getTileSize().y;
	float tileWidth = layer.getMap()->getTileSize().x;
	uint8_t alpha = static_cast<uint8_t>(layer.getOpacity() * 255);

	for (auto &[pos, tileObject] : layer.getTileObjects()) {
		tson::Tileset *tileset = tileObject.getTile()->getTileset();
		bool hasAnimation = tileObject.getTile()->getAnimation().any();
		tson::Rect drawingRect;

		if (!hasAnimation) {
			drawingRect = tileObject.getDrawingRect();
		} else {
			uint32_t ownerId = tileObject.getTile()->getId();
			if (animationUpdateQueue.count(ownerId) == 0) {
				animationUpdateQueue[ownerId] = &tileObject.getTile()->getAnimation();
			}
			uint32_t tileId = animationUpdateQueue[ownerId]->getCurrentTileId();
			tson::Tile *animatedTile = tileset->getTile(tileId);
			drawingRect = animatedTile->getDrawingRect();
		}

		SDL_Texture *texture = nullptr;

		fs::path tilesetPath = getImagePath(*tileObject.getTile());
		texture = getTexture(tilesetPath.generic_string());

		if (texture != nullptr) {
			src.x = drawingRect.x;
			src.y = drawingRect.y;
			src.w = drawingRect.width;
			src.h = drawingRect.height;

			dst.x = tileObject.getPosition().x;
			dst.y = tileObject.getPosition().y + (16 * offset_y);
			dst.w = tileWidth;
			dst.h = tileHeight;

			SDL_Point center = {src.w / 2, src.h / 2};
			SDL_Rect dstCenter = dst;
			dstCenter.x += center.x;
			dstCenter.y += center.y;

			SDL_SetTextureAlphaMod(texture, alpha);
			SDL_RenderCopy(ClientGame::renderer, texture, &src, &dst);
		}
	}
}

void Map::drawImageLayer(tson::Layer &layer) {
	SDL_Texture *texture = nullptr;
	fs::path imagePath = getImagePath(layer.getImage()).relative_path();
	texture = getTexture(imagePath);

	if (texture != nullptr) {
		SDL_RenderCopy(ClientGame::renderer, texture, nullptr, nullptr);
	} else {
		std::cout << "Failed to load texture for image layer: " << imagePath << std::endl;
	}
}

void Map::drawObjectLayer(tson::Layer &layer) {
	for (auto &[pos, tileObject] : layer.getTileObjects()) {
		tson::Rect rect = tileObject.getDrawingRect();
		SDL_Rect block = {static_cast<int>(tileObject.getPosition().x), static_cast<int>(tileObject.getPosition().y),
		                  rect.width, rect.height};

		SDL_SetRenderDrawColor(ClientGame::renderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(ClientGame::renderer, &block);
		SDL_SetRenderDrawColor(ClientGame::renderer, 0, 0, 0, 255);
	}
}

void Map::updateAnimations() {
	for (auto &[id, animation] : animationUpdateQueue) {
		animation->update(30);
	}
}

void Map::drawLoadingBar(int progressBarsComplete) {
	tson::Layer *loadingBar = currentMap->getLayer("loading");
	for (int i = 0; i < progressBarsComplete; i++) {
		drawTileLayer(*loadingBar, i);
	}
}

fs::path Map::getImagePath(std::string const &image) {
	// expect the exe to be in the build/Release or build/Debug directory
	fs::path fullPath = fs::path("../../maps") / image;
	return fullPath;
}

fs::path Map::getImagePath(tson::Tile &tile) {
	// expect the exe to be in the build/Release or build/Debug directory
	fs::path fullPath = fs::path("../../maps") / tile.getTileset()->getImagePath().relative_path();
	return fullPath;
}

SDL_Texture *Map::loadTexture(const std::string &image) {
	SDL_Texture *tmp = IMG_LoadTexture(ClientGame::renderer, image.c_str());
	if (tmp == nullptr) {
		spdlog::get("stderr")->error("Failed to load texture: {}", image);
	}
	return tmp;
}

void Map::loadBackground() {
	// load the background image
	tson::Layer *background = currentMap->getLayer("backgroundPicture");
	fs::path imagePath = getImagePath(background->getImage()).relative_path();
	SDL_Texture *texture = loadTexture(imagePath.generic_string());
	if (texture == nullptr) {
		std::cout << "Failed to load background image!" << std::endl;
	}
	tilesetTextures[imagePath] = texture;
}

void Map::loadTilesetTextures() {
	// loop through the tilesets and load the textures
	for (auto &tileset : map->getTilesets()) {
		if (tileset.getImagePath().empty()) {
			continue;
		}
		fs::path tilesetPath = fs::path("../../maps") / tileset.getImagePath().relative_path();
		SDL_Texture *texture = loadTexture(tilesetPath.generic_string());
		if (texture == nullptr) {
			continue;
		}
		tilesetTextures[tilesetPath] = texture;

		// TODO: free the textures in the destructor
	}
}

SDL_Texture *Map::getTexture(fs::path path) {
	return tilesetTextures[path];
}

bool Map::checkCollisions(SDL_Rect *collider) {
	tson::Layer *plattforms = currentMap->getLayer("plattforms");
	int tileSize = currentMap->getTileSize().x;

	// Calculate tile range to check
	int startX = collider->x / tileSize;
	int startY = collider->y / tileSize;
	int endX = (collider->x + collider->w) / tileSize;
	int endY = (collider->y + collider->h) / tileSize;

	// Loop through tiles in the range
	for (int x = startX; x <= endX; ++x) {
		for (int y = startY; y <= endY; ++y) {
			tson::Tile *tileObject = plattforms->getTileData(x, y);
			if (tileObject != nullptr) {
				tson::Rect rect = tileObject->getDrawingRect();
				SDL_Rect block = {x * tileSize, y * tileSize, rect.width, rect.height};

				if (SDL_HasIntersection(collider, &block)) {
					SDL_Rect intersection;
					if (SDL_IntersectRect(collider, &block, &intersection)) {
						if (intersection.w < intersection.h) {
							collider->x += (collider->x < block.x) ? -intersection.w : intersection.w;
						} else {
							collider->y += (collider->y < block.y) ? -intersection.h : intersection.h;
						}
					}
					return true;
				}
			}
		}
	}
	return false;
}

bool Map::isOnPlattform(SDL_Rect *collider) {
	// const SDL_Point point = {collider->x + (collider->w / 2), collider->y + collider->h};

	tson::Layer *plattforms = currentMap->getLayer("plattforms");

	for (auto &[pos, tileObject] : plattforms->getTileObjects()) {
		tson::Rect rect = tileObject.getDrawingRect();
		SDL_Rect block = {static_cast<int>(tileObject.getPosition().x), static_cast<int>(tileObject.getPosition().y),
		                  rect.width, rect.height};

		if (SDL_HasIntersection(collider, &block)) {
			return true;
		}
	}
	return false;
}

bool Map::isInWater(SDL_Rect *collider) {
	const SDL_Point point = {collider->x + (collider->w / 2), collider->y + (collider->h / 2)};

	tson::Layer *water = currentMap->getLayer("water");
	tson::Layer *waterfall = currentMap->getLayer("waterfalls");

	// calculate tile range to check
	int tileSize = currentMap->getTileSize().x;
	int startX = collider->x / tileSize;
	int startY = collider->y / tileSize;
	int endX = (collider->x + collider->w) / tileSize;
	int endY = (collider->y + collider->h) / tileSize;

	for (int x = startX; x <= endX; ++x) {
		for (int y = startY; y <= endY; ++y) {
			tson::Tile *waterTile = water->getTileData(x, y);
			tson::Tile *waterfallTile = waterfall->getTileData(x, y);

			if (waterTile != nullptr) {
				tson::Rect rect = waterTile->getDrawingRect();
				SDL_Rect block = {x * tileSize, y * tileSize, rect.width, rect.height};

				if (SDL_PointInRect(&point, &block)) {
					return true;
				}
			}

			if (waterfallTile != nullptr) {
				tson::Rect rect = waterfallTile->getDrawingRect();
				SDL_Rect block = {x * tileSize, y * tileSize, rect.width, rect.height};

				if (SDL_PointInRect(&point, &block)) {
					return true;
				}
			}
		}
	}
	return false;
}

bool Map::checkLayer(SDL_Rect *collider, std::string layerName) {
	tson::Layer *layer = currentMap->getLayer(layerName);

	// calculate tile range to check
	int tileSize = currentMap->getTileSize().x;
	int startX = collider->x / tileSize;
	int startY = collider->y / tileSize;
	int endX = (collider->x + collider->w) / tileSize;
	int endY = (collider->y + collider->h) / tileSize;

	for (int x = startX; x <= endX; ++x) {
		for (int y = startY; y <= endY; ++y) {
			tson::Tile *tileObject = layer->getTileData(x, y);

			if (tileObject != nullptr) {
				tson::Rect rect = tileObject->getDrawingRect();
				SDL_Rect block = {x * tileSize, y * tileSize, rect.width, rect.height};

				if (SDL_HasIntersection(collider, &block)) {
					return true;
				}
			}
		}
	}
	return false;
}

std::vector<std::pair<uint16_t, uint16_t>> Map::getPlayerSpawnpoints(size_t numPlayers) {
	tson::Layer *spawnpoints = currentMap->getLayer("playerSpawnpoints");

	std::vector<std::pair<std::uint16_t, std::uint16_t>> spawnpointsVec;
	spawnpointsVec.reserve(numPlayers);

	auto it = spawnpoints->getTileObjects().begin();
	for (int i = 0; i < numPlayers; ++i) {
		auto &[pos, tileObject] = *it;
		tson::Vector2f position = tileObject.getPosition();
		spawnpointsVec.emplace_back(static_cast<uint16_t>(position.x), static_cast<uint16_t>(position.y));
		++it;
	}
	return spawnpointsVec;
}

std::vector<std::pair<uint16_t, uint16_t>> *Map::loadWeaponSpawnpoints() {
	tson::Layer *spawnpoints = currentMap->getLayer("weaponSpawnpoints");

	for (auto &[pos, tileObject] : spawnpoints->getTileObjects()) {
		tson::Vector2f position = tileObject.getPosition();
		weaponSpawnpoints.emplace_back(static_cast<uint16_t>(position.x), static_cast<uint16_t>(position.y));
	}
	return &weaponSpawnpoints;
}

} // namespace FishEngine