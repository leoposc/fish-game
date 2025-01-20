#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/ECS/ColliderComponent.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/MockServer.hpp"
#include "spdlog/spdlog.h"

#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

// #include "ECS.hpp"

namespace FishEngine {

void TransformComponent::init() {
	// spdlog::get("console")->debug( "TRANSFORM COMPONENT INIT" );
	velocity.setX(0);
	velocity.setY(0);
}

void TransformComponent::update() {
	// spdlog::get("console")->debug("Transform - before updating position - velocity Y: {}", velocity.getY());
	position += velocity;
	velocity = {0, 0};
}

void TransformComponent::print() {
	spdlog::get("console")->info("Velocity: ({}, {})", velocity.getX(), velocity.getY());
	spdlog::get("console")->info("Position: ({}, {})", position.getX(), position.getY());
}

void TransformComponent::sync(TransformComponent serverTransform) {
	// this->velocity = serverTransform.velocity;
	// this->position.setX(serverTransform.position.getX());
	// this->position.setY(serverTransform.position.getY());
	// std::cout << "Updated to: (" << velocity.getX() << ", " << velocity.getY() << ")\n";
	// std::cout << "Updated to: (" << position.getX() << ", " << position.getY() << ")\n";
}

} // namespace FishEngine

#include <cereal/archives/json.hpp>

CEREAL_REGISTER_TYPE(FishEngine::TransformComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::TransformComponent);
