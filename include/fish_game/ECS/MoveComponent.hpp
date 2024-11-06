#pragma once

// #include "Components.hpp"
#include "ECS.hpp"

// #include "../Vector2D.hpp"
#include "TransformComponent.hpp"

// class TransformComponent;

namespace FishEngine {

class MoveComponent : public TransformComponent {

public:
  using TransformComponent::TransformComponent;

  MoveComponent(int x, int y);

  MoveComponent(int sc);

  MoveComponent(int x, int y, int h, int w, int sc);

  // void init() override;
  // using TransformComponent::init;
  void init() override { TransformComponent::init(); }

  void update() override { TransformComponent::update(); }

  // using TransformComponent::update;

  void up();

  void down();

  void left();

  void right();

  void stop();
};

} // namespace FishEngine