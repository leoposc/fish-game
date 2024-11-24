#include "../include/fish_game/Map.hpp"
#include "../include/fish_game/ClientGame.hpp"
#include "spdlog/spdlog.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>

namespace FishEngine {

void Map::loadMap(fs::path path) {

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

	loadTilesetTextures();

	// TODO load from config
	initialPos.push_back(std::pair(400, 240));
	initialPos.push_back(std::pair(800, 240));
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
	case tson::LayerType::TileLayer: {
		drawTileLayer(layer);
		break;
	}
	default:
		break;
		// spdlog::get("console")->debug( "Layer type not supported!" );
	}
}

void Map::drawTileLayer(tson::Layer &layer) {
	float tileHeight = layer.getMap()->getTileSize().y;
	float tileWidth = layer.getMap()->getTileSize().x;

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
			dst.y = tileObject.getPosition().y;
			dst.w = tileWidth;
			dst.h = tileHeight;

			SDL_Point center = {src.w / 2, src.h / 2};
			SDL_Rect dstCenter = dst;
			dstCenter.x += center.x;
			dstCenter.y += center.y;

			SDL_RenderCopy(ClientGame::renderer, texture, &src, &dst);
		}
	}
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

void Map::loadTilesetTextures() {
	// loop through the tilesets and load the textures
	for (auto &tileset : map->getTilesets()) {
		fs::path tilesetPath = fs::path("../../maps") / tileset.getImagePath().relative_path();
		SDL_Texture *texture = loadTexture(tilesetPath.generic_string());
		tilesetTextures[tilesetPath] = texture;

		// TODO: free the textures in the destructor
	}
}

SDL_Texture *Map::getTexture(fs::path path) {
	return tilesetTextures[path];
}

bool Map::checkPlattformCollisions(SDL_Rect *collider) {
	tson::Layer *plattforms = currentMap->getLayer("plattforms");

	for (auto &[pos, tileObject] : plattforms->getTileObjects()) {
		tson::Rect rect = tileObject.getDrawingRect();
		SDL_Rect block = {static_cast<int>(tileObject.getPosition().x), static_cast<int>(tileObject.getPosition().y),
		                  rect.width, rect.height};

		if (SDL_HasIntersection(collider, &block)) {
			SDL_Rect intersection;
			if (SDL_IntersectRect(collider, &block, &intersection)) {
				if (intersection.w > intersection.h) {
					if (collider->y < block.y) {
						collider->y -= intersection.h;
					} else {
						collider->y += intersection.h;
					}
				} else {
					if (collider->x < block.x) {
						collider->x -= intersection.w;
					} else {
						collider->x += intersection.w;
					}
				}
			}

			return true;
		}

		SDL_SetRenderDrawColor(ClientGame::renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(ClientGame::renderer, &block);
		SDL_SetRenderDrawColor(ClientGame::renderer, 0, 0, 0, 255);
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

	for (auto &[pos, tileObject] : water->getTileObjects()) {
		tson::Rect rect = tileObject.getDrawingRect();
		SDL_Rect block = {static_cast<int>(tileObject.getPosition().x), static_cast<int>(tileObject.getPosition().y),
		                  rect.width, rect.height};

		if (SDL_PointInRect(&point, &block)) {
			return true;
		}

		SDL_SetRenderDrawColor(ClientGame::renderer, 0, 0, 255, 255);
		SDL_RenderDrawRect(ClientGame::renderer, &block);
		SDL_SetRenderDrawColor(ClientGame::renderer, 0, 0, 0, 255);
	}

	for (auto &[pos, tileObject] : waterfall->getTileObjects()) {
		tson::Rect rect = tileObject.getDrawingRect();
		SDL_Rect block = {static_cast<int>(tileObject.getPosition().x), static_cast<int>(tileObject.getPosition().y),
		                  rect.width, rect.height};

		if (SDL_PointInRect(&point, &block)) {
			return true;
		}

		SDL_SetRenderDrawColor(ClientGame::renderer, 0, 0, 255, 255);
		SDL_RenderDrawRect(ClientGame::renderer, &block);
		SDL_SetRenderDrawColor(ClientGame::renderer, 0, 0, 0, 255);
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