#pragma once

#include "ECS.hpp"
// #include "TransformComponent.hpp"

namespace FishEngine {

class TransformComponent;

class WearableComponent : public Component {

  private:
	TransformComponent *transform = nullptr;

	Entity *attachedEntity = nullptr;
	bool isAttached = false;

  public:
	template <class Archive>
	void serialize(Archive &ar) {
		ar(isAttached); // todo: probably need to add a unique pointer
	}

	WearableComponent() = default;
	~WearableComponent() = default;

	void init() override;

	void update() override;

	void attach(Entity *entity);

	void detach();

	void shoot();
};

} // namespace FishEngine