#ifndef COLLIDERCOMPONENT_HPP
#define COLLIDERCOMPONENT_HPP

#include "Components.hpp"
#include "ECS.hpp"

#include <SDL2/SDL.h>
#include <string>

namespace FishEngine
{
    class ColliderComponent : public Component
    {
    public:
        SDL_Rect collider;
        std::string tag;

        TransformComponent *transform;

        void init() override
        {
            if (!entity->hasComponent<TransformComponent>())
            {
                entity->addComponent<TransformComponent>();
            }
            transform = &entity->getComponent<TransformComponent>();
        }

        void update() override
        {
            collider.x = transform->getX();
            collider.y = transform->getY();
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;
        } 
    };
}
#endif // COLLIDERCOMPONENT_HPP