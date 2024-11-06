#include "../../include/fish_game/ECS/TransformComponent.hpp"

#include "../../include/fish_game/Vector2D.hpp"
// #include "Components.hpp"
// #include "ECS.hpp"

namespace FishEngine {

void TransformComponent::init() {
  velocity.setX(0);
  velocity.setY(0);
}

void TransformComponent::update() {
  position += velocity * speed;
  std::cout << "TransformComponent - new pos: " << position.getX() << " "
            << position.getY() << std::endl;
}

} // namespace FishEngine