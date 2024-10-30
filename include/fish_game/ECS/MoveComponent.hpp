#ifndef MOVECOMPONENT_HPP
#define MOVECOMPONENT_HPP

#include "Components.hpp"
#include "../Vector2D.hpp"

namespace FishEngine
{

    class MoveComponent : public Component
    {
        TransformComponent *transform;

    public:
        void init() override
        {
            transform = &entity->getComponent<TransformComponent>();
        }

        void up()
        {
            transform->velocity.setY(-1);
        }

        void down()
        {
            transform->velocity.setY(1);
        }

        void left()
        {
            transform->velocity.setX(-1);
        }

        void right()
        {
            transform->velocity.setX(1);
        }
    };
}

#endif // MOVECOMPONENT_HPP