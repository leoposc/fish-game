#pragma once

#include "ECS/ColliderComponent.hpp"
#include "ECS/MoveComponent.hpp"
#include "Map.hpp"

#include <SDL2/SDL.h>

namespace FishEngine {

namespace Collision {

bool checkCollisions(const SDL_Rect &colliderA, const SDL_Rect &colliderB);

void checkCollisions(std::vector<Entity *> *players, Map *map);

void checkCollisions(std::vector<Entity *> *players, std::vector<Entity *> *projectiles);

void isInWater(std::vector<Entity *> *players, Map *map);

bool checkBack(const Entity &player, const Map &map);

bool checkJoin(const Entity &player, const Map &map);

bool checkHost(const Entity &player, const Map &map);

bool checkStart(const Entity &player, const Map &map);

} // namespace Collision

} // namespace FishEngine
