#include "../../include/fish_game/ECS/GravityComponent.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"

#include "spdlog/spdlog.h"

namespace FishEngine {

constexpr float MAX_GRAVITY = 5.0;

void GravityComponent::init() {
	// spdlog::get("console")->debug( "GRAVITY COMPONENT INIT" );
	if (!entity->hasComponent<TransformComponent>()) {
		spdlog::get("console")->debug("Gravity Component: Creating TransformComponent.");
		entity->addComponent<TransformComponent>();
	}

	transform = &entity->getComponent<TransformComponent>();
}

void GravityComponent::update() {
	// if the entity is not a fish (no move component) or the fish is not in water, apply gravity
	if (!entity->hasComponent<MoveComponent>() || !(entity->getComponent<MoveComponent>().inWater)) {
		force = force < MAX_GRAVITY ? force + 0.1 : MAX_GRAVITY;
		// spdlog::get("console")->debug("GravityComponent - after updating - force: {}", force);
	} else if (entity->getComponent<MoveComponent>().inWater) {
		// do not apply force if the fish is in water, but let it sink for a little bit
		force -= force > 0 ? force * 0.1 : 0;
	}
	transform->setVelocity(transform->getVelocity() + Vector2D(0, force));
}

void GravityComponent::applyForceTo(Entity *e) const {
	// transfer the force to the entity
	e->getComponent<GravityComponent>().applyForce(force);
}

} // namespace FishEngine

#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::GravityComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::GravityComponent);