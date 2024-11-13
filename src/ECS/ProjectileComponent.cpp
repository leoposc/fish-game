#include "../../include/fish_game/ECS/ProjectileComponent.hpp"

#include "../../include/fish_game/ClientGame.hpp"
// // #include "ECS.hpp"
// #include "Components.hpp"
// // #include "TransformComponent.hpp"

#include "../../include/fish_game/Vector2D.hpp"
// #include "Components.hpp"

namespace FishEngine {

ProjectileComponent::ProjectileComponent(int rng, int sp, Vector2D vel) : range(rng), speed(sp), velocity(vel) {}

void ProjectileComponent::init()
{
	transform = &entity->getComponent<TransformComponent>();
	transform->velocity = velocity;
}

void ProjectileComponent::update()
{
	distance += speed;

	if (distance > range) {
		std::cout << "Projectile out of range" << std::endl;
		entity->destroy();
	} else if (transform->position.getX() > ClientGame::camera.x + ClientGame::camera.w
	           || transform->position.getX() < ClientGame::camera.x
	           || transform->position.getY() > ClientGame::camera.y + ClientGame::camera.h
	           || transform->position.getY() < ClientGame::camera.y) {
		std::cout << "Projectile out of bounds" << std::endl;
		entity->destroy();
	}
}

} // namespace FishEngine