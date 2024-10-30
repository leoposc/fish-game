#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include "Game.hpp"

#include <SDL2/SDL.h>

namespace FishEngine
{

    class TextureManager
    {

    public:
        static SDL_Texture *loadTexture(const char *fileName);
    };
} // namespace Texture

#endif // TEXTUREMANAGER_HPP