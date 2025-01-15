#include "../../include/fish_game/ECS/ProjectileComponent.hpp"

#include "../../include/fish_game/ClientGame.hpp"
#include "../../include/fish_game/Vector2D.hpp"

#include "spdlog/spdlog.h"

namespace FishEngine {

ProjectileComponent::ProjectileComponent(int rng, int sp, Vector2D vel) : range(rng), speed(sp), velocity(vel) {}

void ProjectileComponent::init() {
	// spdlog::get("console")->debug("PROJECTILE COMPONENT INIT" );
	transform = &entity->getComponent<TransformComponent>();
	spdlog::get("console")->debug("ProjectileComponent - init {}", (transform->faceRight ? 1.0 : -1.0));
}

void ProjectileComponent::update() {
	transform->velocity = velocity * (transform->faceRight ? 1.0 : -1.0);
	distance += speed;

	if (distance > range) {
		spdlog::get("console")->debug("Projectile out of range");
		entity->destroy();
	}
}

} // namespace FishEngine

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::ProjectileComponent);