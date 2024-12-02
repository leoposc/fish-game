#include "../../include/fish_game/ECS/ProjectileComponent.hpp"

#include "../../include/fish_game/ClientGame.hpp"
#include "../../include/fish_game/Vector2D.hpp"

#include "spdlog/spdlog.h"

namespace FishEngine {

ProjectileComponent::ProjectileComponent(int rng, int sp, Vector2D vel) : range(rng), speed(sp), velocity(vel) {}

void ProjectileComponent::init() {
	// spdlog::get("console")->debug("PROJECTILE COMPONENT INIT" );
	transform = &entity->getComponent<TransformComponent>();
	transform->velocity = velocity;
}

void ProjectileComponent::update() {
	distance += speed;

	if (distance > range) {
		spdlog::get("console")->debug("Projectile out of range");
		entity->destroy();
	} else if (transform->position.getX() > ClientGame::camera.x + ClientGame::camera.w
	           || transform->position.getX() < ClientGame::camera.x
	           || transform->position.getY() > ClientGame::camera.y + ClientGame::camera.h
	           || transform->position.getY() < ClientGame::camera.y) {
		spdlog::get("console")->debug("Projectile out of bounds");
		entity->destroy();
	}
}

} // namespace FishEngine

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::ProjectileComponent);