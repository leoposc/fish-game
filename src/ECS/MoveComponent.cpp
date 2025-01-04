#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/SpriteComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/Vector2D.hpp"

#include "spdlog/spdlog.h"

// #include "MoveComponent.hpp"

namespace FishEngine {

// MoveComponent::MoveComponent() {}

void MoveComponent::init() {
	// spdlog::get("console")->debug( "MOVE COMPONENT INIT");
	if (!entity->hasComponent<TransformComponent>()) {
		spdlog::get("console")->debug("MoveComponent: Creating TransformComponent");
		entity->addComponent<TransformComponent>();
	}
	transform = &entity->getComponent<TransformComponent>();
	transform->velocity.setX(0);
	transform->velocity.setY(0);

	// init sprite component if client
	if (entity->hasComponent<SpriteComponent>()) {
		sprite = &entity->getComponent<SpriteComponent>();
		sprite->play("fishIdle");
	}
}

void MoveComponent::update() {
	// every entity with a MoveComponent ignores gravity while in water
	if (inWater) {
		transform->velocity.setY(0);
	}
}

void MoveComponent::up() {

	if (inWater) {
		transform->velocity.setY(-2);
		canJump = true;
	} else if (canJump) {
		transform->velocity.setY(-5);
		canJump = false;
	}

	if (entity->hasComponent<SpriteComponent>()) {
		sprite->play("fishSwim");
	}
}

void MoveComponent::down() {
	transform->velocity.setY(2);

	if (entity->hasComponent<SpriteComponent>()) {
		sprite->play("fishSwim");
	}
}

void MoveComponent::left() {
	// face left
	transform->faceRight = false;

	if (inWater) {
		transform->velocity.setX(-4);
	} else {
		transform->velocity.setX(-2);
	}

	if (entity->hasComponent<SpriteComponent>()) {
		sprite->play("fishSwim");
	}
}

void MoveComponent::right() {
	// face right
	transform->faceRight = true;

	if (inWater) {
		transform->velocity.setX(4);
	} else {
		transform->velocity.setX(2);
	}

	if (entity->hasComponent<SpriteComponent>()) {
		sprite->play("fishSwim");
	}
}

void MoveComponent::stopX() {
	transform->velocity.setX(0);

	if (entity->hasComponent<SpriteComponent>()) {
		sprite->play("fishIdle");
	}
}

void MoveComponent::stopY() {
	if (inWater) {
		transform->velocity.setY(0);
	}

	if (entity->hasComponent<SpriteComponent>()) {
		sprite->play("fishIdle");
	}
}

} // namespace FishEngine

#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::MoveComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::MoveComponent);