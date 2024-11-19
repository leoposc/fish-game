#pragma once

#include "ECS.hpp"
#include "MoveComponent.hpp"
#include "TransformComponent.hpp"

namespace FishEngine {

class GravityComponent : public Component {

	TransformComponent *transform = nullptr;
	MoveComponent *move = nullptr;

  public:
	GravityComponent() {}

	void init() override;

	void update() override;
};

} // namespace FishEngine