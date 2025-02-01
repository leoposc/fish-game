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
	spdlog::get("cb")->debug(
	    "WearableComponent - position: {} {} ----- serverside: {}", static_cast<int>(transform->getPosition().getX()),
	    static_cast<int>(transform->getPosition().getY()), entity->hasComponent<SpriteComponent>());
	if (attached) {
		// copy the values of the attached entity to the entity
		TransformComponent *attached_transform = &attachedEntity->getComponent<TransformComponent>();
		transform->setPosition(attached_transform->getPosition());
		transform->setVelocity(attached_transform->getVelocity());
		transform->setFaceRight(attached_transform->isFacingRight());

		// offset the position to the center of the entity
		float x = attached_transform->getPosition().getX() + attached_transform->getWidth() / 3;
		float y = attached_transform->getPosition().getY() + attached_transform->getHeight() / 3;
		transform->setPosition({x, y});
	} else {
		momentum -= momentum * FRICTION;
		transform->setPosition(transform->getPosition() + momentum);
	}
}

void WearableComponent::attach(Entity *entity) {
	if (entity == nullptr) {
		spdlog::get("console")->error("WearableComponent - attach - entity is nullptr");
		return;
	}

	spdlog::get("console")->debug("WearableComponent - attach");
	attachedEntity = entity;
	attachedID = entity->getID();
	attached = true;
	needsUpdate = true;

	MusicPlayer::getInstance().playEquipSound();

	// check if code is executed on the clientside
	if (this->entity->hasComponent<SpriteComponent>()) {
		this->entity->getComponent<SpriteComponent>().setTexture("pistol");
	}
	this->entity->getComponent<TransformComponent>().setWidth(18);
	this->entity->getComponent<TransformComponent>().setHeight(13);
}

void WearableComponent::detach() {
	if (attachedEntity == nullptr) {
		spdlog::get("console")->error(
		    "WearableComponent - detach - attachedEntity is nullptr. This happend on the server: {}",
		    entity->hasComponent<ServerComponent>());
		return;
	}

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

// code is only executed on the server side
void WearableComponent::shoot() {
	if (ammunition > 0) {

		Manager &manager = *entity->getManager();
		Entity &projectile(manager.addEntity());
		std::pair<std::uint16_t, std::uint16_t> pos(transform->getPosition().getX(), transform->getPosition().getY());
		ServerGenerator::forProjectile(projectile, pos, transform->isFacingRight());
		spdlog::get("console")->info("WearableComponent - shoot - projectile ID: {}", projectile.getID());
		// decrease the ammunition
		ammunition--;
	}
}

} // namespace FishEngine
