#include "../include/fish_game/Map.hpp"
#include "../include/fish_game/TextureManager.hpp"

#include <SDL2/SDL.h>

namespace FishEngine
{
    Map::Map(SDL_Renderer *renderer)
    {
        plattforms = Texture::TextureManager::LoadTexture("assets/dirt.png", renderer);
        water = Texture::TextureManager::LoadTexture("assets/water.png", renderer);
        trees = Texture::TextureManager::LoadTexture("assets/grass.png", renderer);
    }

    void Map::loadMap()
    {
    }

    void Map::drawMap()
    {
    }
}