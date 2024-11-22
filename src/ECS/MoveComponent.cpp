#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/Vector2D.hpp"
// #include "MoveComponent.hpp"

namespace FishEngine {

// MoveComponent::MoveComponent() {}

void MoveComponent::init() {
	// std::cout << "MOVE COMPONENT INIT" << std::endl;
	if (!entity->hasComponent<TransformComponent>()) {
		std::cout << "MoveComponent: Creating TransformComponent" << std::endl;
		entity->addComponent<TransformComponent>();
	}
	transform = &entity->getComponent<TransformComponent>();
	transform->velocity.setX(0);
	transform->velocity.setY(0);
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
}

void MoveComponent::down() {
	transform->velocity.setY(2);
}

void MoveComponent::left() {
	if (inWater) {
		transform->velocity.setX(-4);
	} else {
		transform->velocity.setX(-2);
	}
}

void MoveComponent::right() {
	if (inWater) {
		transform->velocity.setX(4);
	} else {
		transform->velocity.setX(2);
	}
}

void MoveComponent::stop() {
	transform->velocity.setX(0);
	transform->velocity.setY(0);
}

} // namespace FishEngine