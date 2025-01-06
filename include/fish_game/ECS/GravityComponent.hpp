#pragma once

#include "ECS.hpp"
#include "MoveComponent.hpp"
#include "TransformComponent.hpp"

namespace FishEngine {

class GravityComponent : public Component {

	TransformComponent *transform = nullptr;
	MoveComponent *move = nullptr;
	float force = 0;

  public:
	GravityComponent() = default;
	~GravityComponent() = default;

	void init() override;

	void update() override;

	void touchGround() { force = 0; }

	void applyForce(float f) { force += f; }

	void copyForceFrom(Entity *e);
};

} // namespace FishEngine