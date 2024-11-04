#ifndef MAP_HPP
#define MAP_HPP

#include "../include/tileson.hpp"

#include <SDL2/SDL.h>
#include <array>
#include <string>

namespace FishEngine {
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
    for (auto &[pos, tileObject] :
         currentMap->getLayer("plattforms")->getTileObjects()) {
      (*mapArray)[std::get<1>(pos)][std::get<0>(pos)] = 1;
    }

        // loop through the water layer and set the water tiles to 2
    for (auto &[pos, tileObject] :
         currentMap->getLayer("water")->getTileObjects()) {
      (*mapArray)[std::get<1>(pos)][std::get<0>(pos)] = 2;
    }

    return mapArray;
  }

  void loadTilesetTextures();

  SDL_Texture *getTexture(fs::path path);

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
};
} // namespace FishEngine

#endif // MAP_HPP