#include "../../include/fish_game/ECS/WearableComponent.hpp"
#include "../../include/fish_game/ECS/ColliderComponent.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/ProjectileComponent.hpp"
#include "../../include/fish_game/ECS/SpriteComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "spdlog/spdlog.h"

namespace FishEngine {

void WearableComponent::init() {
	// spdlog::get("console")->debug( "WearableComponent - init" );
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
	spdlog::get("console")->debug("WearableComponent - attach");
	attachedEntity = entity;
	isAttached = true;
}

void WearableComponent::detach() {
	spdlog::get("console")->debug("WearableComponent - detach");
	attachedEntity = nullptr;
	isAttached = false;
}

void WearableComponent::shoot() {
	spdlog::get("console")->debug("WearableComponent - shoot");
}

} // namespace FishEngine

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::WearableComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::WearableComponent);