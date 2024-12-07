#pragma once

// #include "../Game.hpp"
// // #include "ECS.hpp"
// #include "Components.hpp"
#include "../Vector2D.hpp"
#include "ECS.hpp"
#include "TransformComponent.hpp"
// #include "Components.hpp"

namespace FishEngine {

class ProjectileComponent : public Component {
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

  private:
	TransformComponent *transform;

	int range = 0;
	int speed = 0;
	int distance = 0;
	Vector2D velocity;
};

} // namespace FishEngine