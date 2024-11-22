#pragma once

#include "ECS/ColliderComponent.hpp"
#include "ECS/MoveComponent.hpp"
#include "Map.hpp"

#include <SDL2/SDL.h>

namespace FishEngine {

class Collision {
  public:
	static void test() {}

	static void checkWaterCollisions(std::vector<Entity *> *players, Map *map);

	static void checkPlattformCollisions(std::vector<Entity *> *players, Map *map);
};

} // namespace FishEngine
