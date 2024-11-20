#pragma once

#include "ECS/ColliderComponent.hpp"

#include <SDL2/SDL.h>

namespace FishEngine {

class Collision {
  public:
	static bool AABB(const SDL_Rect &recA, const SDL_Rect &recB);
	static bool AABB(const ColliderComponent &colA, const ColliderComponent &colB);
};
} // namespace FishEngine
