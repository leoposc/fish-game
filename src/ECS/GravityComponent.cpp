#include "../../include/fish_game/ECS/GravityComponent.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"

namespace FishEngine {

constexpr float MAX_GRAVITY = 5.0;

void GravityComponent::init() {
	std::cout << "GRAVITY COMPONENT INIT" << std::endl;
	if (!entity->hasComponent<MoveComponent>()) {
		std::cout << "Gravity Component: Creating MoveComponent." << std::endl;
		entity->addComponent<MoveComponent>();
	}
	if (!entity->hasComponent<TransformComponent>()) {
		std::cout << "Gravity Component: Creating TransformComponent." << std::endl;
		entity->addComponent<TransformComponent>();
	}

	transform = &entity->getComponent<TransformComponent>();

	move = &entity->getComponent<MoveComponent>();
}

void GravityComponent::update() {
	// if the entity is not a fish (no move component) or the fish is not in water, apply gravity
	// std::cout << "GRAVITY COMPONENT UPDATE" << std::endl;
	// std::cout << !entity->hasComponent<MoveComponent>() << std::endl;
	// std::cout << !move->inWater << std::endl;

	if (!entity->hasComponent<MoveComponent>() || !move->inWater) {
		float y = transform->velocity.getY();
		transform->velocity.setY(y < MAX_GRAVITY ? y + 0.1 : MAX_GRAVITY);
	}
}

} // namespace FishEngine