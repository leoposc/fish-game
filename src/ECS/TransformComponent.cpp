#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/MockServer.hpp"
// #include "Components.hpp"
// #include "ECS.hpp"

namespace FishEngine {

void TransformComponent::init() {
	// std::cout << "TRANSFORM COMPONENT INITIALIZED with the addr: " << this << std::endl;
	velocity.setX(0);
	velocity.setY(0);
}

void ServerTransformComponent::update() {
	position += velocity * speed;
	MockServer::getInstance().enqueuePosition(position);
}

void ClientTransformComponent::update() {
	FishEngine::Vector2D newPos;

	if (MockServer::getInstance().pollPosition(newPos)) {
		position = newPos;
	}
}

} // namespace FishEngine