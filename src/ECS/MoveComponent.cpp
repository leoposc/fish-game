#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/Vector2D.hpp"
// #include "MoveComponent.hpp"

namespace FishEngine {

// void MoveComponent::init() {
//   // transform = &entity->getComponent<MoveComponent>();
//   TransformComponent::init();
// }

MoveComponent::MoveComponent(int x, int y) : ServerTransformComponent(x, y) {}

MoveComponent::MoveComponent(int sc) : ServerTransformComponent(sc) {}

MoveComponent::MoveComponent(int x, int y, int h, int w, float sc) : ServerTransformComponent(x, y, h, w, sc)
{
	// std::cout << "MoveComponent created! Position at: " << x << " " << y
	// << std::endl;
}

void MoveComponent::update()
{
	// add gravity
	// if (!inWater) {
	// 	velocity.setY(1);
	// }
	ServerTransformComponent::update();
}

void MoveComponent::up()
{
	// if (inWater) {
	velocity.setY(-1);
	// }
}

void MoveComponent::down()
{
	// if (inWater) {
	velocity.setY(1);
	// }
}

void MoveComponent::left()
{
	if (inWater) {
		velocity.setX(-2);
	} else {
		velocity.setX(-1);
	}
}

void MoveComponent::right()
{
	if (inWater) {
		velocity.setX(2);
	} else {
		velocity.setX(1);
	}
}

void MoveComponent::stop()
{
	velocity.setX(0);
	velocity.setY(0);
}

} // namespace FishEngine