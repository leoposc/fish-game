#pragma once

#include "ECS.hpp"
#include "TransformComponent.hpp"

namespace FishEngine {

class MoveComponent : public Component {

	TransformComponent *transform = nullptr;

  public:
	bool inWater = false;

	MoveComponent() {}

	void init() override;

	void update() override;

	void up();

	void down();

	void left();

	void right();

	void stop();
};

} // namespace FishEngine