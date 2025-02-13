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
	void save(Archive &ar) const {
		ar(alive);
	}

	template <class Archive>
	void load(Archive &ar) {
		ar(alive);

		// if (!alive) {
		// 	startTimer = SDL_GetTicks();
		// }
	}

	HealthComponent() : alive(true) {}

	void init() override;

	void update() override;

	void takeDamage();

	bool isAlive() const { return alive; }
};

} // namespace FishEngine