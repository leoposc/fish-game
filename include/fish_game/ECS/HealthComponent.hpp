#pragma once

#include "ECS.hpp"

#include <cereal/types/polymorphic.hpp>

namespace FishEngine {

class TransformComponent;

class HealthComponent : public Component {

	TransformComponent *transform = nullptr;

	bool alive = true;

	Uint32 startTimer;

  public:
	template <class Archive>
	void serialize(Archive &ar) {
		ar(alive);
	}

	HealthComponent() : alive(true) {}

	void init() override;

	void update() override;

	void takeDamage();

	bool isAlive() { return alive; }
};

} // namespace FishEngine