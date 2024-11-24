#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/ECS/ColliderComponent.hpp"
#include "../../include/fish_game/MockServer.hpp"
#include "spdlog/spdlog.h"

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

} // namespace FishEngine