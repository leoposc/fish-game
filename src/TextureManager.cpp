#include "../include/fish_game/TextureManager.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace Texture
{
    SDL_Texture *TextureManager::LoadTexture(const char *fileName, SDL_Renderer *renderer)
    {
        SDL_Surface *tmpSurface = IMG_Load(fileName);
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);

        return tex;
    }
}