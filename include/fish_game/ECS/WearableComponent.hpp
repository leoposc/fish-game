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
	WearableComponent() = default;
	~WearableComponent() = default;

	void init() override;

	void update() override;

	void attach(Entity *entity);

	void detach();

	void shoot();
};

} // namespace FishEngine