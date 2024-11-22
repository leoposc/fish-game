#pragma once

#include "ECS/ColliderComponent.hpp"
#include "ECS/MoveComponent.hpp"
#include "Map.hpp"

#include <SDL2/SDL.h>

namespace FishEngine {

namespace Collision {

void checkWaterCollisions(std::vector<Entity *> *players, Map *map);

void checkPlattformCollisions(std::vector<Entity *> *players, Map *map);

} // namespace Collision

} // namespace FishEngine
