#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_HPP

#include "Components.hpp"
#include "../TextureManager.hpp"
#include <SDL2/SDL.h>

namespace FishEngine
{

    class SpriteComponent : public Component
    {
        TransformComponent *transform;
        SDL_Texture *texture;
        SDL_Rect srcRect, dstRect;

    public:
        SpriteComponent() = default;
        SpriteComponent(const char *path)
        {
            setTexture(path);
        }

        ~SpriteComponent()
        {
            SDL_DestroyTexture(texture);
        }

        void setTexture(const char *path)
        {
            texture = TextureManager::loadTexture(path);
        }

        void init() override
        {
            transform = &entity->getComponent<TransformComponent>();

            srcRect.x = srcRect.y = 0;
            srcRect.w = srcRect.h = 32;
            dstRect.w = dstRect.h = 32;
        }

        void update() override
        {
            dstRect.x = (int)transform->getX();
            dstRect.y = (int)transform->getY();
            dstRect.w = transform->width * transform->scale;
            dstRect.h = transform->height * transform->scale;
        }

        void draw() override
        {
            // Texture::Manager::draw(texture, srcRect, dstRect);
        }
    };

} // namespace FishEngine

#endif // SPRITECOMPONENT_HPP