#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/ECS/ColliderComponent.hpp"
#include "../../include/fish_game/MockServer.hpp"
// #include "ECS.hpp"

namespace FishEngine {

void TransformComponent::init() {
	// std::cout << "TRANSFORM COMPONENT INIT" << std::endl;
	velocity.setX(0);
	velocity.setY(0);
}

void TransformComponent::update() {
	position += velocity;
}

} // namespace FishEngine