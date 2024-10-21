#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "ECS.hpp"

class PositionComponent : public Component
{
    int xpos = 0;
    int ypos = 0;

public:
    int getX() { return xpos; }
    int getY() { return ypos; }

    void setPos(int x, int y)
    {
        xpos = x;
        ypos = y;
    }
};

#endif // COMPONENTS_HPP