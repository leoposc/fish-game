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

	void loadMap(fs::path path, bool hasBackground = true);

	void drawMap();

	void drawLayer(tson::Layer &layer);

	void drawTileLayer(tson::Layer &layer, int offset_y = 0);

	void drawImageLayer(tson::Layer &layer);

	void drawObjectLayer(tson::Layer &layer);

	void updateAnimations();

	void drawLoadingBar(int progressBarsComplete);

	// fetch the path to the tileset
	fs::path getTilePath(int id);

	fs::path getImagePath(std::string const &image);

	fs::path getImagePath(tson::Tile &tile);

	SDL_Texture *loadTexture(const std::string &image);

	/**
	 * @brief: idea: pass colllider components to the map and check for collision
	 */
	bool checkCollisions(SDL_Rect *collider);

	/**
	 * @brief: checks if the player is on the specifies layer
	 * @details: the map is divided into layers, the map needs a layer which matches the provided layerName
	 */
	bool checkLayer(const SDL_Rect *collider, const std::string layerName) const;

	/**
	 * @brief: check if the player is in water
	 * @param: SDL_Rect *collider   the collider of the player
	 */
	bool isInWater(SDL_Rect *collider);

	/**
	 * @brief: check if the player is on a plattform
	 * @param: SDL_Rect *collider   the collider of the player
	 */
	bool isOnPlattform(SDL_Rect *collider);

	std::vector<std::pair<uint16_t, uint16_t>> getPlayerSpawnpoints();

	std::vector<std::pair<uint16_t, uint16_t>> *loadWeaponSpawnpoints();

	void loadBackground();

	void loadTilesetTextures();

	SDL_Texture *getTexture(fs::path path);

	std::vector<std::pair<std::uint16_t, std::uint16_t>> getInitialPos() { return initialPos; }

  private:
	SDL_Rect src, dst;

	std::unordered_map<fs::path, SDL_Texture *> tilesetTextures;

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

	std::unordered_map<uint32_t, tson::Animation *> animationUpdateQueue;

	std::vector<std::pair<std::uint16_t, std::uint16_t>> initialPos;

	std::vector<std::pair<uint16_t, uint16_t>> weaponSpawnpoints;
};

} // namespace FishEngine
