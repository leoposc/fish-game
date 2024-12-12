#include "../../include/fish_game/ECS/WearableComponent.hpp"
#include "../../include/fish_game/ECS/ColliderComponent.hpp"
#include "../../include/fish_game/ECS/ComponentsGenerator.hpp"
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
	if (attached) {
		// copy the values of the attached entity to the entity
		transform->position = attachedEntity->getComponent<TransformComponent>().position;
	} else {
		transform->velocity -= transform->velocity * 0.1;
	}
}

void WearableComponent::attach(Entity *entity) {
	spdlog::get("console")->debug("WearableComponent - attach");
	attachedEntity = entity;
	attached = true;
}

void WearableComponent::detach() {
	spdlog::get("console")->debug("WearableComponent - detach");

	// transfer the velocity of the attached entity to the entity
	transform->velocity = attachedEntity->getComponent<TransformComponent>().velocity;

	// reset
	attachedEntity = nullptr;
	attached = false;
}

void WearableComponent::shoot() {
	static int i = 50;
	spdlog::get("console")->debug("WearableComponent - shoot");
	Manager &manager = *entity->getManager();
	Entity &projectile(manager.addEntity(i++));
	std::pair<std::uint16_t, std::uint16_t> pos(transform->position.getX(), transform->position.getY());
	ClientGenerator::forProjectile(projectile, pos);
}

} // namespace FishEngine

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::WearableComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::WearableComponent);