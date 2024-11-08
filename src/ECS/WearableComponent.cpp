#include "../../include/fish_game/ECS/WearableComponent.hpp"
// #include "../../include/fish_game/ECS/ECS.hpp"

namespace FishEngine {

// WearableComponent::WearableComponent(/* args */) {}

// WearableComponent::~WearableComponent() {}

void WearableComponent::init() {
	if (!entity->hasComponent<TransformComponent>()) {
		entity->addComponent<TransformComponent>();
	}
	transform = &entity->getComponent<TransformComponent>();
}

// void WearableComponent::update() {}

void WearableComponent::attach(Entity *entity) {
	attachedEntity = entity;
	isAttached = true;
	transform = &attachedEntity->getComponent<TransformComponent>();
}

void WearableComponent::detach() {
	attachedEntity = nullptr;
	isAttached = false;
	TransformComponent *tmp = transform;
	transform = &entity->getComponent<TransformComponent>();
	// copy the values of the attached entity to the entity
	// TODO: PROBLEM: the values are not copied, but the pointer is copied
	// create a deep copy of the transform component
	*transform = *tmp;
}

} // namespace FishEngine