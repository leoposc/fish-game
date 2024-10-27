#ifndef MAP_HPP
#define MAP_HPP

#include "../include/tileson.hpp"

#include <SDL2/SDL.h>
#include <string>

namespace FishEngine
{
    class Map
    {
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

        SDL_Texture *getTexture(const std::string &image);

    private:
        SDL_Rect src, dst;

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