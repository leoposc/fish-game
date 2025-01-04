#pragma once

#include "ECS.hpp"
// #include "TransformComponent.hpp"

namespace FishEngine {

class SpriteComponent;
class TransformComponent;

class MoveComponent : public Component {

	TransformComponent *transform = nullptr;
	SpriteComponent *sprite = nullptr;

	bool canJump = false;

  public:
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