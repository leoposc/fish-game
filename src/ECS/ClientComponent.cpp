#include "../../include/fish_game/ECS/ClientComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/MockServer.hpp"

#include "spdlog/spdlog.h"

namespace FishEngine {

void ClientComponent::init() {
	spdlog::get("console")->debug("Client Component Initialized");

	transform = &entity->getComponent<TransformComponent>();
}

void ClientComponent::update() {
	Vector2D newPos;

	if (MockServer::getInstance().pollPosition(newPos)) {
		transform->position = newPos;
	}
}

} // namespace FishEngine
