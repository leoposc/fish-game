#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/Vector2D.hpp"
// #include "MoveComponent.hpp"

namespace FishEngine {

// MoveComponent::MoveComponent() {}

void MoveComponent::init() {
	std::cout << "MOVE COMPONENT INIT" << std::endl;
	transform = &entity->getComponent<TransformComponent>();
	transform->velocity.setX(0);
	transform->velocity.setY(0);
}

void MoveComponent::update() {
	// add gravity
	// if (!inWater) {
	// 	velocity.setY(1);
	// }
}

void MoveComponent::up() {
	// if (inWater) {
	transform->velocity.setY(-1);
	// }
}

void MoveComponent::down() {
	// if (inWater) {
	transform->velocity.setY(1);
	// }
}

void MoveComponent::left() {
	if (inWater) {
		transform->velocity.setX(-2);
	} else {
		transform->velocity.setX(-1);
	}
}

void MoveComponent::right() {
	if (inWater) {
		transform->velocity.setX(2);
	} else {
		transform->velocity.setX(1);
	}
}

void MoveComponent::stop() {
	transform->velocity.setX(0);
	transform->velocity.setY(0);
}

} // namespace FishEngine