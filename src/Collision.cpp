#include "../include/fish_game/Collision.hpp"

namespace FishEngine {

namespace Collision {

bool checkCollision(const SDL_Rect &colliderA, const SDL_Rect &colliderB) {
	return SDL_HasIntersection(&colliderA, &colliderB);
}

void checkWaterCollisions(std::unordered_map<uint8_t, Entity *> *players, Map *map) {
	for (auto &[id, player] : *players) {
		player->getComponent<MoveComponent>().inWater =
		    map->isInWater(&player->getComponent<ColliderComponent>().collider);
	}
}

void checkPlattformCollisions(std::unordered_map<uint8_t, Entity *> *players, Map *serverMap) {
	for (auto &[id, player] : *players) {
		if (serverMap->checkPlattformCollisions(&player->getComponent<ColliderComponent>().collider)) {

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

bool checkExit(Entity *player, Map *map) {
	return map->checkLayer(&player->getComponent<ColliderComponent>().collider, "exit");
}

bool checkStart(Entity *player, Map *map) {
	return map->checkLayer(&player->getComponent<ColliderComponent>().collider, "start");
}

} // namespace Collision

} // namespace FishEngine