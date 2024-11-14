#include "../../include/fish_game/ECS/ClientComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/MockServer.hpp"

namespace FishEngine {

void ClientComponent::init() {
	std::cout << "CLIENT COMPONENT INIT" << std::endl;
	transform = &entity->getComponent<TransformComponent>();
}

void ClientComponent::update() {
	Vector2D newPos;

	if (MockServer::getInstance().pollPosition(newPos)) {
		transform->position = newPos;
	}
}

} // namespace FishEngine
