#include "../include/fish_game/Collision.hpp"
#include "../include/fish_game/ECS/HealthComponent.hpp"
#include "../include/fish_game/ECS/TransformComponent.hpp"

namespace FishEngine {

namespace Collision {

bool checkCollisions(const SDL_Rect &colliderA, const SDL_Rect &colliderB) {
	return SDL_HasIntersection(&colliderA, &colliderB);
}

void checkCollisions(std::vector<Entity *> *players, Map *serverMap) {
	for (auto &player : *players) {
		if (serverMap->checkCollisions(&player->getComponent<ColliderComponent>().collider)) {

			// check if player is moving downwards
			if (player->getComponent<TransformComponent>().velocity.getY() > 0) {
				player->getComponent<TransformComponent>().velocity.setY(0);
			}
			SDL_Rect *collider = &player->getComponent<ColliderComponent>().collider;
			player->getComponent<TransformComponent>().position = {static_cast<float>(collider->x),
			                                                       static_cast<float>(collider->y)};
		}
	}
}

void checkCollisions(std::vector<Entity *> *players, std::vector<Entity *> *projectiles) {
	for (auto &player : *players) {
		for (auto &projectile : *projectiles) {
			if (checkCollisions(player->getComponent<ColliderComponent>().collider,
			                    projectile->getComponent<ColliderComponent>().collider)) {
				player->getComponent<HealthComponent>().takeDamage();
				projectile->destroy();
			}
		}
	}
}

void isInWater(std::vector<Entity *> *players, Map *map) {
	for (auto &player : *players) {
		player->getComponent<MoveComponent>().inWater =
		    map->isInWater(&player->getComponent<ColliderComponent>().collider);
	}
}

bool checkExit(Entity *player, Map *map) {
	return map->checkLayer(&player->getComponent<ColliderComponent>().collider, "exit");
}

bool checkStart(Entity *player, Map *map) {
	return map->checkLayer(&player->getComponent<ColliderComponent>().collider, "start");
}

} // namespace Collision

} // namespace FishEngine