#include "../include/fish_game/Map.hpp"
#include "../include/fish_game/Game.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>

namespace FishEngine {

// Map::Map() = default;

// Map::~Map() = default;

void Map::loadMap(fs::path path) {

	if (!fs::exists(path)) {
		std::cout << "Map file does not exist!" << std::endl;
		return;
	}

	map = tileson.parse(path);

	if (map->getStatus() == tson::ParseStatus::OK) {
		std::cout << "Map loaded successfully!" << std::endl;
	} else {
		std::cout << "Map loading failed!" << std::endl;
		return;
	}

	loadTilesetTextures();
}

void Map::drawMap() {
	positionOffset = {0, 0};
	currentMap = map.get();

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
		std::cout << "Layer type not supported!" << std::endl;
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
		// if (tileset->getType() == tson::TilesetType::ImageCollectionTileset) {
		//   tson::Tile *tile = tileObject.getTile();
		//   if (tile->getFlipFlags() != tson::TileFlipFlags::None) {
		//     tile = tileset->getTile(tile->getId());
		//   }
		//   fs::path tilesetPath = getImagePath(*tile);
		//   texture = getTexture(tilesetPath.generic_string());
		// }

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

			SDL_RenderCopy(Game::renderer, texture, &src, &dst);
		}
	}
}

fs::path Map::getImagePath(tson::Tile &tile) {
	// expect the exe to be in the build/Release or build/Debug directory
	fs::path fullPath = fs::path("../../maps") / tile.getTileset()->getImagePath().relative_path();
	return fullPath;
}

SDL_Texture *Map::loadTexture(const std::string &image) {
	SDL_Texture *tmp = IMG_LoadTexture(Game::renderer, image.c_str());
	if (tmp == nullptr) {
		std::cout << "Failed to load texture: " << image << std::endl;
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
			return true;
		}

		SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(Game::renderer, &block);
		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
	}
	return false;
}

bool Map::isInWater(SDL_Rect *collider) {
	tson::Layer *water = currentMap->getLayer("water");
	tson::Layer *waterfall = currentMap->getLayer("waterfalls");

	for (auto &[pos, tileObject] : water->getTileObjects()) {
		tson::Rect rect = tileObject.getDrawingRect();
		SDL_Rect block = {static_cast<int>(tileObject.getPosition().x), static_cast<int>(tileObject.getPosition().y),
		                  rect.width, rect.height};

		if (SDL_HasIntersection(collider, &block)) {
			return true;
		}

		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 255, 255);
		SDL_RenderDrawRect(Game::renderer, &block);
		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
	}

	for (auto &[pos, tileObject] : waterfall->getTileObjects()) {
		tson::Rect rect = tileObject.getDrawingRect();
		SDL_Rect block = {static_cast<int>(tileObject.getPosition().x), static_cast<int>(tileObject.getPosition().y),
		                  rect.width, rect.height};

		if (SDL_HasIntersection(collider, &block)) {
			return true;
		}

		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 255, 255);
		SDL_RenderDrawRect(Game::renderer, &block);
		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
	}
	return false;
}

} // namespace FishEngine