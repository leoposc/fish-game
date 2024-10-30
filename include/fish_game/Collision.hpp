#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Game.hpp"

#include <SDL2/SDL.h>

namespace FishEngine
{

    class Collision
    {
    public:
        static bool AABB(const SDL_Rect &recA, const SDL_Rect &recB);
    };
}

#endif // COLLISION_HPP
