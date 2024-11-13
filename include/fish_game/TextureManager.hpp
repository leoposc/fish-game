#pragma once

// #include "Game.hpp"

#include <SDL2/SDL.h>
#include <filesystem>

namespace fs = std::filesystem;
namespace FishEngine {
class TextureManager {

public:
  static SDL_Texture *loadTexture(fs::path path);
  static void draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dst,
                   SDL_RendererFlip flip);
};
} // namespace FishEngine