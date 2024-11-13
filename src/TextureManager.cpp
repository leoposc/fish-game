#include "../include/fish_game/TextureManager.hpp"
#include "../include/fish_game/Game.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace FishEngine {
SDL_Texture *TextureManager::loadTexture(fs::path path) {
  SDL_Surface *tmpSurface = IMG_Load(path.c_str());
  SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
  SDL_FreeSurface(tmpSurface);

  return tex;
}

void TextureManager::draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dst,
                          SDL_RendererFlip flip) {
  SDL_RenderCopyEx(Game::renderer, tex, &src, &dst, 0.0, NULL, flip);
}
} // namespace FishEngine