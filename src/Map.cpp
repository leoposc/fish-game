#include "../include/fish_game/Map.hpp"
#include "../include/fish_game/Game.hpp"

#include <SDL2/SDL.h>
#include <fstream>
#include <iostream>

namespace FishEngine
{
    // Map::Map() = default;

    // Map::~Map() = default;

    void Map::loadMap(fs::path path)
    {
        map = tileson.parse(path);

        if (map->getStatus() == tson::ParseStatus::OK)
        {
            std::cout << "Map loaded successfully!" << std::endl;
        }
        else
        {
            std::cout << "Map loading failed!" << std::endl;
            return;
        }

        // get plattforms, water and background
        // plattformsLayer = map->getLayer("plattforms");
        // waterLayer = map->getLayer("water");
        // treesLayer = map->getLayer("trees");
    }

    void Map::drawMap()
    {
        positionOffset = {0, 0};
        currentMap = map.get();

        for (auto &layer : currentMap->getLayers())
        {
            drawLayer(layer);
        }
    }

    fs::path Map::getTilePath(int id)
    {
        // fetch the path to the tileset
        // use the id to get the right tileset

        fs::path path = map->getTileset("tileset")->getImagePath();
        return path;
    }

    void Map::drawLayer(tson::Layer &layer)
    {
        switch (layer.getType())
        {
        case tson::LayerType::TileLayer:
        {
            drawTileLayer(layer);
            break;
        }
        default:
            std::cout << "Layer type not supported!" << std::endl;
        }
    }

    void Map::drawTileLayer(tson::Layer &layer)
    {
        float tileHeight = layer.getMap()->getTileSize().y;
        float tileWidth = layer.getMap()->getTileSize().x;

        for (auto &[pos, tileObject] : layer.getTileObjects())
        {
            tson::Tileset *tileset = tileObject.getTile()->getTileset();
            bool hasAnimation = tileObject.getTile()->getAnimation().any();
            tson::Rect drawingRect;

            if (!hasAnimation)
            {
                drawingRect = tileObject.getDrawingRect();
            }
            else
            {
                uint32_t ownerId = tileObject.getTile()->getId();
                if (animationUpdateQueue.count(ownerId) == 0)
                {
                    animationUpdateQueue[ownerId] = &tileObject.getTile()->getAnimation();
                }
                uint32_t tileId = animationUpdateQueue[ownerId]->getCurrentTileId();
                tson::Tile *animatedTile = tileset->getTile(tileId);
                drawingRect = animatedTile->getDrawingRect();
            }

            SDL_Texture *texture = nullptr;
            if (tileset->getType() == tson::TilesetType::ImageCollectionTileset)
            {
                tson::Tile *tile = tileObject.getTile();
                if (tile->getFlipFlags() != tson::TileFlipFlags::None)
                {
                    tile = tileset->getTile(tile->getId());
                }
                fs::path tilesetPath = getImagePath(*tile);
                texture = getTexture(tilesetPath.generic_string());
            }

            if (texture != nullptr)
            {
                src.x = drawingRect.x;
                src.y = drawingRect.y;
                src.w = drawingRect.width;
                src.h = drawingRect.height;

                dst.x = tileObject.getPosition().x;
                dst.y = tileObject.getPosition().y;
                dst.w = tileWidth;
                dst.h = tileHeight;

                SDL_RenderCopy(Game::renderer, texture, &src, &dst);
            }
        }
    }

    fs::path Map::getImagePath(tson::Tile &tile)
    {
    }

    SDL_Texture *Map::getTexture(const std::string &image)
    {
    }
} // namespace FishEngine
