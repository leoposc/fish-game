#pragma once

#include "../Vector2D.hpp"
#include "ECS.hpp"
#include "TransformComponent.hpp"

namespace FishEngine {

class ProjectileComponent : public Component {
	TransformComponent *transform;

	int range = 0;
	int speed = 0;
	int distance = 0;
	Vector2D velocity;

  public:
	template <class Archive>
	void serialize(Archive &ar) {
		ar(range, speed, distance, velocity);
	}

	ProjectileComponent() = default;
	~ProjectileComponent() = default;
	ProjectileComponent(int rng, int sp, Vector2D vel);

	void init() override;

	void update() override;
};

} // namespace FishEngine