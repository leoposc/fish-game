#include "../../include/fish_game/ECS/ServerComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/MockServer.hpp"

#include "spdlog/spdlog.h"

namespace FishEngine {

void ServerComponent::init() {
	// spdlog::get("console")->debug( "SERVER COMPONENT INIT" );
	transform = &entity->getComponent<TransformComponent>();
}

void ServerComponent::update() {
	// enqueue new position after it was updated
	MockServer::getInstance().enqueuePosition(transform->position);
}

} // namespace FishEngine

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::ServerComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::ServerComponent);