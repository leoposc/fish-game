#include "../include/fish_game/Collision.hpp"
#include "../include/fish_game/ECS/GravityComponent.hpp"
#include "../include/fish_game/ECS/HealthComponent.hpp"
#include "../include/fish_game/ECS/TransformComponent.hpp"
#include "fish_game/ECS/MoveComponent.hpp"
#include "fish_game/MusicPlayer.hpp"

namespace FishEngine {

class ProjectileComponent;

namespace Collision {

bool checkCollisions(const SDL_Rect &colliderA, const SDL_Rect &colliderB) {
	return SDL_HasIntersection(&colliderA, &colliderB);
}

bool checkCollisionsAux(Entity *player, Map *map) {
	bool collided = map->checkCollisions(&player->getComponent<ColliderComponent>().collider);
	if (collided) {
		// check if player is moving downwards
		if (player->getComponent<TransformComponent>().getVelocity().getY() > 0) {
			player->getComponent<GravityComponent>().touchGround();
			// if (player->hasComponent<MoveComponent>())
			// 	player->getComponent<MoveComponent>().stopY();
		}
		SDL_Rect *collider = &player->getComponent<ColliderComponent>().collider;
		player->getComponent<TransformComponent>().setPosition(
		    {static_cast<float>(collider->x), static_cast<float>(collider->y)});
	}
	return collided;
}

void checkCollisions(std::vector<Entity *> *entities, Map *map) {
	for (auto &entity : *entities) {
		if (map->checkCollisions(&entity->getComponent<ColliderComponent>().collider)) {

			// check if entity is moving downwards
			if (entity->getComponent<TransformComponent>().getVelocity().getY() > 0) {
				entity->getComponent<GravityComponent>().touchGround();
			}
			SDL_Rect *collider = &entity->getComponent<ColliderComponent>().collider;
			entity->getComponent<TransformComponent>().setPosition(
			    {static_cast<float>(collider->x), static_cast<float>(collider->y)});

			// destroy if entity is a projectile
			if (entity->hasComponent<ProjectileComponent>())
				entity->destroy();

			// if (entity->hasComponent<MoveComponent>()) {
			// 	entity->getComponent<MoveComponent>().collisionStop();
			// }
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
		bool inWater = map->isInWater(&player->getComponent<ColliderComponent>().collider);
		if (!player->getComponent<MoveComponent>().inWater && inWater) {
			MusicPlayer::getInstance().playSplashSound();
		}
		player->getComponent<MoveComponent>().inWater = inWater;
	}
}

bool checkBack(const Entity &player, const Map &map) {
	return map.checkLayer(&player.getComponent<ColliderComponent>().collider, "back");
}

bool checkJoin(const Entity &player, const Map &map) {
	return map.checkLayer(&player.getComponent<ColliderComponent>().collider, "join");
}

bool checkHost(const Entity &player, const Map &map) {
	return map.checkLayer(&player.getComponent<ColliderComponent>().collider, "host");
}

bool checkStart(const Entity &player, const Map &map) {
	return map.checkLayer(&player.getComponent<ColliderComponent>().collider, "start");
}

} // namespace Collision

} // namespace FishEngine
