#pragma once

#include "../tileson.hpp"

#include <SDL2/SDL.h>
#include <array>
#include <string>

namespace FishEngine {

/**
 * @brief: class to load and draw a map
 * @details: the map is loaded using the tileson library
 * the map is drawn using the SDL2 library
 */

class Map {
  public:
	Map() = default;
	~Map() = default;

	void loadMap(fs::path path);

	void drawMap();

	void drawLayer(tson::Layer &layer);

	void drawTileLayer(tson::Layer &layer);

	// fetch the path to the tileset
	fs::path getTilePath(int id);

	fs::path getImagePath(tson::Tile &tile);

	SDL_Texture *loadTexture(const std::string &image);

	/**
	 * @brief: idea: pass colllider components to the map and check for collision
	 */
	bool checkPlattformCollisions(SDL_Rect *collider);

	/**
	 * @brief: check if the player is in water
	 * @param: SDL_Rect *collider   the collider of the player
	 */
	bool isInWater(SDL_Rect *collider);

	/**
	 * @brief: get the spawn positions of the players
	 * @details: the spawn positions are stored in the map file and are returned in pixel points
	 * @param: size_t numPlayers   the number of players
	 * @return copy of the vector of pairs of uint16_t (since playerSpawnpoints is only need once
	 * per initialization there is no need to make it a class attribute)
	 */
	std::vector<std::pair<uint16_t, uint16_t>> getPlayerSpawnpoints(size_t numPlayers);

	/**
	 * @brief: get the spawn positions of the weapons
	 * @details: the spawn positions are stored in the map file and are returned in pixel points
	 * @return pointer to a vector of pairs of uint16_t, points towards class attribute weaponSpawnpoints
	 */
	std::vector<std::pair<uint16_t, uint16_t>> *getWeaponSpawnpoints();
	/*
	 * @brief: create a two dimensional array of the size of the map
	 * @param: void
	 * @details: the array is used to store type of the tile at the position (e.g.
	 * water=2, plattform=1, tree)
	 */
	template <typename T, size_t Row, size_t Col>
	std::array<std::array<uint8_t, Col>, Row> *initMapArray() {

		tson::Vector2i mapSize = currentMap->getSize();
		size_t rows = mapSize.y;
		size_t cols = mapSize.x;
		// TODO: dynamic allocation, delete in destructor? or use std::unique_pt?
		std::array<std::array<uint8_t, Col>, Row> *mapArray =
		    new std::array<std::array<uint8_t, mapSize.x>, mapSize.y>();
		for (size_t i = 0; i < rows; ++i) {
			for (size_t j = 0; j < cols; ++j) {
				(*mapArray)[i][j] = 0;
			}
		}

		// loop through the plattforms layer and set the plattform tiles to 1
		for (auto &[pos, tileObject] : currentMap->getLayer("plattforms")->getTileObjects()) {
			(*mapArray)[std::get<1>(pos)][std::get<0>(pos)] = 1;
		}

		// loop through the water layer and set the water tiles to 2
		for (auto &[pos, tileObject] : currentMap->getLayer("water")->getTileObjects()) {
			(*mapArray)[std::get<1>(pos)][std::get<0>(pos)] = 2;
		}

		return mapArray;
	}

	void loadTilesetTextures();

	SDL_Texture *getTexture(fs::path path);

	std::vector<std::pair<std::uint16_t, std::uint16_t>> getInitialPos() { return initialPos; }

  private:
	SDL_Rect src, dst;

	std::map<fs::path, SDL_Texture *> tilesetTextures;

	SDL_Texture *plattforms;
	SDL_Texture *water;
	SDL_Texture *trees;

	tson::Layer *plattformsLayer;
	tson::Layer *waterLayer;
	tson::Layer *treesLayer;

	tson::Tileson tileson;
	std::unique_ptr<tson::Map> map = nullptr;
	tson::Map *currentMap = nullptr;

	tson::Vector2i tileSize;
	tson::Vector2i mapSize;
	tson::Vector2i mapScale;
	tson::Vector2i positionOffset{0, 0};

	std::map<uint32_t, tson::Animation *> animationUpdateQueue;

	std::vector<std::pair<std::uint16_t, std::uint16_t>> initialPos;

	std::vector<std::pair<uint16_t, uint16_t>> weaponSpawnpoints;
};

} // namespace FishEngine