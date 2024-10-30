#include "../include/fish_game/Collision.hpp"

namespace FishEngine
{
    bool Collision::AABB(const SDL_Rect &recA, const SDL_Rect &recB)
    {
        if (recA.x + recA.w >= recB.x &&
            recB.x + recB.w >= recA.x &&
            recA.y + recA.h >= recB.y &&
            recB.y + recB.h >= recA.y)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}