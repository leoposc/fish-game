#include "../../include/fish_game/ECS/TransformComponent.hpp"

#include "../../include/fish_game/Vector2D.hpp"
// #include "Components.hpp"
// #include "ECS.hpp"

namespace FishEngine {

void TransformComponent::init() {
	// std::cout << "TRANSFORM COMPONENT INITIALIZED with the addr: " << this << std::endl;
	velocity.setX(0);
	velocity.setY(0);
}

void TransformComponent::update() {
	// std::cout << "TransformComponent - velocity: " << velocity.getX() << " " << velocity.getY() << std::endl;
	position += velocity * speed;
}

} // namespace FishEngine