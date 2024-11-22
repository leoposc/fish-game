#include "../../include/fish_game/ECS/ServerComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/MockServer.hpp"

namespace FishEngine {

void ServerComponent::init() {
	// std::cout << "SERVER COMPONENT INIT" << std::endl;
	transform = &entity->getComponent<TransformComponent>();
}

void ServerComponent::update() {
	// enqueue new position after it was updated
	MockServer::getInstance().enqueuePosition(transform->position);
}

} // namespace FishEngine