#include "../include/fish_game/Collision.hpp"

namespace FishEngine {

namespace Collision {

void checkWaterCollisions(std::vector<Entity *> *players, Map *map) {
	for (auto &player : *players) {
		player->getComponent<MoveComponent>().inWater =
		    map->isInWater(&player->getComponent<ColliderComponent>().collider);
	}
}

void checkPlattformCollisions(std::vector<Entity *> *players, Map *serverMap) {
	for (auto &player : *players) {
		if (serverMap->checkPlattformCollisions(&player->getComponent<ColliderComponent>().collider)) {
			// check if player is moving downwards
			if (player->getComponent<TransformComponent>().velocity.getY() > 0) {
				player->getComponent<TransformComponent>().velocity.setY(0);
			}
		}
	}
}

} // namespace Collision

} // namespace FishEngine