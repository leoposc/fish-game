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
  if (!blocked) {
    position += velocity * speed;
  }
}

} // namespace FishEngine