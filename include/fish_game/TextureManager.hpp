#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include "Game.hpp"

#include <SDL2/SDL.h>

namespace Texture
{

    class TextureManager
    {

    public:
        static SDL_Texture *LoadTexture(const char *fileName, SDL_Renderer *renderer);
    };
} // namespace Texture

#endif // TEXTUREMANAGER_HPP