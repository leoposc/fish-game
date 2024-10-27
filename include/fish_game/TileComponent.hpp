#ifndef TILECOMPONENT_HPP
#define TILECOMPONENT_HPP

#include "Components.hpp"

class TileComponent : public Component
{
public:
    TileComponent() = default;

    // void init() override
    // {
    //     entity->addComponent<TransformComponent>();
    //     transform = &entity->getComponent<TransformComponent>();
    // }

    // void update() override
    // {
    //     transform->position.x = xpos;
    //     transform->position.y = ypos;
    //     transform->width = width;
    //     transform->height = height;
    // }
};

#endif // TILECOMPONENT_HPP