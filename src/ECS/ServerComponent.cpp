

#include "../../include/fish_game/ECS/ServerComponent.hpp"
#include "../../include/fish_game/Auxiliary.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "fish_game/ServerGame.hpp"

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>
#include <fstream>
#include <spdlog/spdlog.h>

namespace FishEngine {

void ServerComponent::init() {
	transform = &entity->getComponent<TransformComponent>();
}

void ServerComponent::setEvent(const SDL_Event event) {
	this->event = event;
}

} // namespace FishEngine
