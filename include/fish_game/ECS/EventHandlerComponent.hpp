#pragma once

#include "../ServerGame.hpp"
#include "ECS.hpp"

#include <SDL2/SDL.h>

namespace FishEngine {

class MoveComponent;

class EventHandlerComponent : public Component {

  public:
	MoveComponent *move;

	EventHandlerComponent() = default;
	~EventHandlerComponent() = default;

	void init() override;

	void update() override;
};

} // namespace FishEngine