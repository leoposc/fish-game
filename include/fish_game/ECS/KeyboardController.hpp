#pragma once

#include "../ClientGame.hpp"
#include "ECS.hpp"

// #include "MoveComponent.hpp"
// #include "SpriteComponent.hpp"

#include <SDL2/SDL.h>

namespace FishEngine {

class KeyboardController : public Component {

  public:
	KeyboardController() = default;
	~KeyboardController() = default;

	void init() override;

	void update() override;
};

} // namespace FishEngine