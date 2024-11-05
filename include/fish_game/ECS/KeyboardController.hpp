#pragma once

#include "../Game.hpp"
#include "ECS.hpp"

// #include "MoveComponent.hpp"
// #include "SpriteComponent.hpp"

#include <SDL2/SDL.h>

namespace FishEngine {

class SpriteComponent;
class MoveComponent;

class KeyboardController : public Component {

public:
  MoveComponent *move;
  SpriteComponent *sprite;

  KeyboardController() = default;
  ~KeyboardController() = default;

  void init() override;

  void update() override;
};

} // namespace FishEngine