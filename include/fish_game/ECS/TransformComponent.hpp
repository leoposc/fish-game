#pragma once

#include "../Vector2D.hpp"
#include "ECS.hpp"

namespace FishEngine {

class TransformComponent : public Component {

public:
  Vector2D velocity;
  Vector2D position;

  int speed = 3;

  // TODO: read from file/ tileson
  int height = 16;
  int width = 16;

  float scale = 1;

  bool blocked = false;

  TransformComponent() : position(0, 0) {}

  TransformComponent(int x, int y) : position(x, y) {}

  TransformComponent(int sc) : scale(sc), position(0, 0) {}

  TransformComponent(int x, int y, int h, int w, float sc)
      : position(x, y), height(h), width(w), scale(sc) {}

  int getX() { return position.getX(); }
  int getY() { return position.getY(); }
  void setX(int x) { position.setX(x); }
  void setY(int y) { position.setY(y); }

  void setPos(int x, int y) {
    position.setX(x);
    position.setY(y);
  }

  void init() override;
};

class ClientTransformComponent : public TransformComponent {
  public:
    using TransformComponent::TransformComponent;

    void update() override;
};

class ServerTransformComponent : public TransformComponent {
  public:
    using TransformComponent::TransformComponent;
    
    void update() override;
};


} // namespace FishEngine