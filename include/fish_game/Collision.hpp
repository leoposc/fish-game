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

bool checkBack(Entity *player, Map *map);

bool checkJoin(Entity *player, Map *map);

bool checkHost(Entity *player, Map *map);

bool checkStart(Entity *player, Map *map);

} // namespace Collision

} // namespace FishEngine
