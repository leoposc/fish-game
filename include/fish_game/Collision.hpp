#pragma once

#include "ECS/ColliderComponent.hpp"
#include "ECS/MoveComponent.hpp"
#include "Map.hpp"

#include <SDL2/SDL.h>

namespace FishEngine {

namespace Collision {

bool checkCollision(const SDL_Rect &colliderA, const SDL_Rect &colliderB);

void checkWaterCollisions(std::unordered_map<uint8_t, Entity *> *players, Map *map);

void checkPlattformCollisions(std::unordered_map<uint8_t, Entity *> *players, Map *map);

bool checkExit(Entity *player, Map *map);

bool checkStart(Entity *player, Map *map);

} // namespace Collision

} // namespace FishEngine
