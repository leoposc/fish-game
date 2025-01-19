#include "../../include/fish_game/ECS/ServerComponent.hpp"
#include "../../include/fish_game/Auxiliary.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/MockServer.hpp"
#include "fish_game/ServerGame.hpp"

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>
#include <fstream>
#include <spdlog/spdlog.h>

namespace FishEngine {

void ServerComponent::init() {
	// spdlog::get("console")->debug( "SERVER COMPONENT INIT" );
	transform = &entity->getComponent<TransformComponent>();
}

void ServerComponent::setEvent(SDL_Event event) {
	spdlog::get("console")->debug("Event set");
	this->event = event;
	spdlog::get("console")->debug("Server breakpoint");
	spdlog::get("console")->debug("Event type: {}, Event keysym.sym {}", event.type, event.key.keysym.sym);
}

} // namespace FishEngine

CEREAL_REGISTER_TYPE(FishEngine::ServerComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::ServerComponent);
