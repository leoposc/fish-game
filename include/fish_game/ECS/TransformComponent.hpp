#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP

#include "Components.hpp"
#include "../Vector2D.hpp"

namespace FishEngine
{
    class TransformComponent : public Component
    {
        Vector2D position;

        int speed = 3;

    public:
        Vector2D velocity;

        int height = 32;
        int width = 32;
        int scale = 1;

        TransformComponent() : position(0, 0) {}
        TransformComponent(int x, int y) : position(x, y) {}

        int getX() { return position.getX(); }
        int getY() { return position.getY(); }
        void setX(int x) { position.setX(x); }
        void setY(int y) { position.setY(y); }

        void setPos(int x, int y)
        {
            position.setX(x);
            position.setY(y);

            // position += (velocity * speed);
        }

        void init() override
        {
            velocity.setX(0);
            velocity.setY(0);
        }
    };

} // namespace FishEngine

#endif // TRANSFORMCOMPONENT_HPP