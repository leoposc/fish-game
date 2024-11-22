#include "../../include/fish_game/ECS/WearableComponent.hpp"
#include "../../include/fish_game/ECS/ColliderComponent.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/ProjectileComponent.hpp"
#include "../../include/fish_game/ECS/SpriteComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"

namespace FishEngine {

void WearableComponent::init() {
	// std::cout << "WearableComponent - init" << std::endl;
	if (!entity->hasComponent<TransformComponent>()) {
		entity->addComponent<TransformComponent>();
	}
	transform = &entity->getComponent<TransformComponent>();
}

void WearableComponent::update() {
	if (isAttached) {
		// copy the values of the attached entity to the entity
		transform->position = attachedEntity->getComponent<TransformComponent>().position;
	}
}

void WearableComponent::attach(Entity *entity) {
	std::cout << "WearableComponent - attach" << std::endl;
	attachedEntity = entity;
	isAttached = true;
}

void WearableComponent::detach() {
	std::cout << "WearableComponent - detach" << std::endl;
	attachedEntity = nullptr;
	isAttached = false;
}

void WearableComponent::shoot() {
	std::cout << "WearableComponent - shoot" << std::endl;
}

} // namespace FishEngine