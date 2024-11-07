#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/Vector2D.hpp"
// #include "MoveComponent.hpp"

namespace FishEngine {

// void MoveComponent::init() {
//   // transform = &entity->getComponent<MoveComponent>();
//   TransformComponent::init();
// }

MoveComponent::MoveComponent(int x, int y) : TransformComponent(x, y) {}

MoveComponent::MoveComponent(int sc) : TransformComponent(sc) {}

MoveComponent::MoveComponent(int x, int y, int h, int w, float sc) : TransformComponent(x, y, h, w, sc) {
	// std::cout << "MoveComponent created! Position at: " << x << " " << y
	// << std::endl;
}

void MoveComponent::up() {
	velocity.setY(-1);
}

void MoveComponent::down() {
	velocity.setY(1);
}

void MoveComponent::left() {
	velocity.setX(-1);
}

void MoveComponent::right() {
	velocity.setX(1);
}

void MoveComponent::stop() {
	velocity.setX(0);
	velocity.setY(0);
}

} // namespace FishEngine