#include "../../include/fish_game/ECS/GravityComponent.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"

#include "spdlog/spdlog.h"

namespace FishEngine {

constexpr float MAX_GRAVITY = 5.0;

void GravityComponent::init() {
	// spdlog::get("console")->debug( "GRAVITY COMPONENT INIT" );
	if (!entity->hasComponent<MoveComponent>()) {
		spdlog::get("console")->debug("Gravity Component: Creating MoveComponent.");
		entity->addComponent<MoveComponent>();
	}
	if (!entity->hasComponent<TransformComponent>()) {
		spdlog::get("console")->debug("Gravity Component: Creating TransformComponent.");
		entity->addComponent<TransformComponent>();
	}

	transform = &entity->getComponent<TransformComponent>();

	move = &entity->getComponent<MoveComponent>();
}

void GravityComponent::update() {
	// if the entity is not a fish (no move component) or the fish is not in water, apply gravity
	// spdlog::get("console")->debug( "GRAVITY COMPONENT UPDATE" );
	// spdlog::get("console")->debug( !entity->hasComponent<MoveComponent>() );
	// spdlog::get("console")->debug( !move->inWater );

	if (!entity->hasComponent<MoveComponent>() || !move->inWater) {
		float y = transform->velocity.getY();
		transform->velocity.setY(y < MAX_GRAVITY ? y + 0.1 : MAX_GRAVITY);
	}
}

} // namespace FishEngine

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::GravityComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::GravityComponent);