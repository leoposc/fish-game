#include "../include/fish_game/TextureManager.hpp"
#include "../include/fish_game/Game.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace FishEngine
{
    SDL_Texture *TextureManager::loadTexture(const char *fileName)
    {
        SDL_Surface *tmpSurface = IMG_Load(fileName);
        SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);

        return tex;
    }
} // namespace FishEngine