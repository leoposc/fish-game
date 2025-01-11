#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/ECS/ColliderComponent.hpp"
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
	position += velocity;
}

void TransformComponent::print() {
	std::cout << "Velocity: (" << velocity.getX() << ", " << velocity.getY() << ")\n";
	std::cout << "Position: (" << position.getX() << ", " << position.getY() << ")\n";
}

void TransformComponent::sync(TransformComponent serverTransform) {
	this->velocity = serverTransform.velocity;
	this->position = serverTransform.position;
}

} // namespace FishEngine

#include <cereal/archives/json.hpp>

CEREAL_REGISTER_TYPE(FishEngine::TransformComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::TransformComponent);
