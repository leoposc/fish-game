#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/GravityComponent.hpp"
#include "../../include/fish_game/ECS/SpriteComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/MusicPlayer.hpp"
#include "../../include/fish_game/Vector2D.hpp"

#include "spdlog/spdlog.h"

constexpr float MAX_JUMP_FORCE = -7.0;

namespace FishEngine {

void MoveComponent::init() {
	// spdlog::get("console")->debug( "MOVE COMPONENT INIT");
	if (!entity->hasComponent<TransformComponent>()) {
		spdlog::get("console")->debug("MoveComponent: Creating TransformComponent");
		entity->addComponent<TransformComponent>();
	}
	transform = &entity->getComponent<TransformComponent>();
	gravityC = &entity->getComponent<GravityComponent>();
	transform->setVelocity({0, 0});

	// init sprite component if client
	if (entity->hasComponent<SpriteComponent>()) {
		sprite = &entity->getComponent<SpriteComponent>();
		sprite->play("fishIdle");
	}
}

void MoveComponent::update() {
	// every entity with a MoveComponent ignores gravity while in water and can jump
	if (inWater) {
		canJump = true;
	}
	transform->setVelocity(transform->getVelocity() + velocity);
}

void MoveComponent::up() {
	if (inWater) {
		velocity.setY(-2);
	} else if (canJump) {
		MusicPlayer::getInstance().playJumpSound();
		velocity.setY(0);
		gravityC->applyForce(MAX_JUMP_FORCE);
		canJump = false;
	}

	if (entity->hasComponent<SpriteComponent>()) {
		sprite->play("fishSwim");
	}
}

void MoveComponent::down() {

	velocity.setY(2);
	if (entity->hasComponent<SpriteComponent>()) {
		sprite->play("fishSwim");
	}
}

void MoveComponent::left() {
	transform->setFaceRight(false);

	if (inWater) {
		velocity.setX(-4);
	} else {
		velocity.setX(-2);
	}

	if (entity->hasComponent<SpriteComponent>()) {
		sprite->play("fishSwim");
	}
}

void MoveComponent::right() {
	transform->setFaceRight(true);

	if (inWater) {
		velocity.setX(4);
	} else {
		velocity.setX(2);
	}

	if (entity->hasComponent<SpriteComponent>()) {
		sprite->play("fishSwim");
	}
}

void MoveComponent::stopX() {
	velocity.setX(0);

	if (entity->hasComponent<SpriteComponent>()) {
		sprite->play("fishIdle");
	}
}

void MoveComponent::stopY() {
	// if (inWater) {
	velocity.setY(0);
	// }
	// velocity.setY(0);

	if (entity->hasComponent<SpriteComponent>()) {
		sprite->play("fishIdle");
	}
}

void MoveComponent::collisionStop() {
	velocity.setX(0);
	velocity.setY(0);
}

bool MoveComponent::isInWater() const {
	return inWater;
}

void MoveComponent::setInWater(bool inWater) {
	inWater = inWater;
}

float MoveComponent::getJumpForce() const {
	return jumpForce;
}

void MoveComponent::setJumpForce(float jumpForce) {
	this->jumpForce = jumpForce;
}

Vector2D MoveComponent::getVelocity() const {
	return velocity;
}

void MoveComponent::setVelocity(const Vector2D velocity) {
	this->velocity = velocity;
}

} // namespace FishEngine

#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::MoveComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::MoveComponent);
