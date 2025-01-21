#include "../../include/fish_game/ECS/WearableComponent.hpp"
#include "../../include/fish_game/ECS/ColliderComponent.hpp"
#include "../../include/fish_game/ECS/ComponentsGenerator.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/ProjectileComponent.hpp"
#include "../../include/fish_game/ECS/SpriteComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/MusicPlayer.hpp"
#include "spdlog/spdlog.h"

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::WearableComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::WearableComponent);

const float FRICTION = 0.02;
const float THROW_SPEED_FACTOR = 3;
const float THROW_UPWARDS_BY = -5;

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
		TransformComponent *attached_transform = &attachedEntity->getComponent<TransformComponent>();
		transform->position = attached_transform->position;
		transform->velocity = attached_transform->velocity;
		transform->faceRight = attached_transform->faceRight;

		// offset the position to the center of the entity
		transform->position.setX(attached_transform->position.getX() + attached_transform->width / 3);
		transform->position.setY(attached_transform->position.getY() + attached_transform->height / 3);
	} else {
		momentum -= momentum * FRICTION;
		transform->position += momentum;
	}
}

void WearableComponent::attach(Entity *entity) {
	spdlog::get("console")->debug("WearableComponent - attach");
	attachedEntity = entity;
	attachedID = entity->getID();
	attached = true;
	needsUpdate = true;

	// check if code is executed on the clientside
	if (this->entity->hasComponent<SpriteComponent>()) {
		this->entity->getComponent<SpriteComponent>().setTexture("pistol");
	}
	this->entity->getComponent<TransformComponent>().width = 18;
	this->entity->getComponent<TransformComponent>().height = 13;
}

void WearableComponent::detach() {
	spdlog::get("console")->debug("WearableComponent - detach : velocity: {} {}",
	                              attachedEntity->getComponent<MoveComponent>().velocity.getX(),
	                              attachedEntity->getComponent<MoveComponent>().velocity.getY());

	// transfer the velocity of the attached entity to the entity
	momentum = attachedEntity->getComponent<MoveComponent>().velocity;
	momentum.setX(momentum.getX() * THROW_SPEED_FACTOR);
	momentum.setY(momentum.getY() + THROW_UPWARDS_BY);
	// entity->getComponent<GravityComponent>().copyForceFrom(attachedEntity);

	// reset
	attachedEntity = nullptr;
	attachedID = -1;
	attached = false;
	needsUpdate = true;
}

void WearableComponent::shoot() {
	static int i = 50;

	if (ammunition > 0) {
		spdlog::get("console")->debug("WearableComponent - shoot");
		Manager &manager = *entity->getManager();
		MusicPlayer::getInstance().playShootSound();
		Entity &projectile(manager.addEntity(i++));
		std::pair<std::uint16_t, std::uint16_t> pos(transform->position.getX(), transform->position.getY());
		ClientGenerator::forProjectile(projectile, pos, transform->faceRight);

		// decrease the ammunition
		ammunition--;
	}
}

void WearableComponent::setForce(float f) {
	entity->getComponent<TransformComponent>() = f;
}

} // namespace FishEngine
