#include "../include/fish_game/Object.hpp"
#include "../include/fish_game/TextureManager.hpp"

#include <SDL2/SDL.h>

Object::Object(const char *texturePath, SDL_Renderer *renderer, int x, int y) : xpos(x), ypos(y), renderer(renderer)
{
    texture = Texture::TextureManager::LoadTexture(texturePath, renderer);
}

void Object::update()
{
    xpos = 0;
    ypos = 0;

    srcRect.h = 32;
    srcRect.w = 32;
    srcRect.x = 0;
    srcRect.y = 0;

    dstRect.x = xpos;
    dstRect.y = ypos;
    dstRect.w = srcRect.w * 2;
    dstRect.h = srcRect.h * 2;
}

void Object::render()
{
    SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}