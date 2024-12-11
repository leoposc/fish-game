#pragma once

#include "ECS.hpp"
#include "TransformComponent.hpp"

namespace FishEngine {

class MoveComponent : public Component {

	TransformComponent *transform = nullptr;

	bool canJump = false;

  public:
	template <class Archive>
	void serialize(Archive &ar) {}

	bool inWater = false;

	MoveComponent() = default;
	~MoveComponent() = default;

	void init() override;

	void update() override;

	void up();

	void down();

	void left();

	void right();

	void stopX();

	void stopY();
};

} // namespace FishEngine