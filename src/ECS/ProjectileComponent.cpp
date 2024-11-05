#include "../../include/fish_game/ECS/ProjectileComponent.hpp"

#include "../../include/fish_game/Game.hpp"
// // #include "ECS.hpp"
// #include "Components.hpp"
// // #include "TransformComponent.hpp"

#include "../../include/fish_game/Vector2D.hpp"
// #include "Components.hpp"

namespace FishEngine {

ProjectileComponent::ProjectileComponent(int rng, int sp, Vector2D vel)
    : range(rng), speed(sp), velocity(vel) {}

void ProjectileComponent::init() {
  transform = &entity->getComponent<TransformComponent>();
  transform->velocity = velocity;
}

void ProjectileComponent::update() {
  distance += speed;

  if (distance > range) {
    std::cout << "Projectile out of range" << std::endl;
    entity->destroy();
  } else if (transform->position.getX() > Game::camera.x + Game::camera.w ||
             transform->position.getX() < Game::camera.x ||
             transform->position.getY() > Game::camera.y + Game::camera.h ||
             transform->position.getY() < Game::camera.y) {
    std::cout << "Projectile out of bounds" << std::endl;
    entity->destroy();
  }
}

} // namespace FishEngine