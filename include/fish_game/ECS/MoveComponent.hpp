#pragma once

// #include "Components.hpp"
#include "ECS.hpp"

// #include "../Vector2D.hpp"
#include "TransformComponent.hpp"

// class TransformComponent;

namespace FishEngine {

class MoveComponent : public ServerTransformComponent {

  public:
	bool inWater = false;
	using ServerTransformComponent::ServerTransformComponent;

	MoveComponent(int x, int y);

	MoveComponent(int sc);

	MoveComponent(int x, int y, int h, int w, float sc);

	// void init() override;
	// using TransformComponent::init;
	void init() override { ServerTransformComponent::init(); }

	void update() override;

	// using TransformComponent::update;

	void up();

	void down();

	void left();

	void right();

	void stop();
};

} // namespace FishEngine