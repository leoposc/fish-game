#include "../../include/fish_game/ECS/ClientComponent.hpp"
#include "../../include/fish_game/Auxiliary.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/MockServer.hpp"

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>
#include <fstream>

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

void ClientComponent::sendEvent(SDL_Event &event) {
	// serilize the event and send it to the server
	{
		std::ofstream os("event.json");
		cereal::JSONOutputArchive archive(os);
		archive(event);
	}
}

} // namespace FishEngine

CEREAL_REGISTER_TYPE(FishEngine::ClientComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::ClientComponent)