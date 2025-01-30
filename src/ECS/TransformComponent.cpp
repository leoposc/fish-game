#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/ECS/ColliderComponent.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/MockServer.hpp"
#include "spdlog/spdlog.h"

#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

namespace FishEngine {

void TransformComponent::init() {
	velocity = {0, 0};
}

void TransformComponent::update() {
	position += velocity;
	velocity = {0, 0};
}

void TransformComponent::print() {
	spdlog::get("console")->info("Velocity: ({}, {})", velocity.getX(), velocity.getY());
	spdlog::get("console")->info("Position: ({}, {})", position.getX(), position.getY());
}

} // namespace FishEngine

#include <cereal/archives/json.hpp>

CEREAL_REGISTER_TYPE(FishEngine::TransformComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::TransformComponent);
