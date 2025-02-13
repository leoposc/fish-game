#pragma once

#include "ECS.hpp"
// #include "TransformComponent.hpp"

namespace FishEngine {

class TransformComponent;

class WearableComponent : public Component {

  private:
	// association properties for game logic
	TransformComponent *transform = nullptr;
	Entity *attachedEntity = nullptr;
	bool attached = false;

	// association properties for serialization
	uint8_t attachedID;
	bool needsUpdate = false;

	size_t ammunition = 5;
	Vector2D momentum = {0, 0};

  public:
	template <class Archive>
	void serialize(Archive &ar) {
		ar(ammunition);
	}

	WearableComponent() = default;
	~WearableComponent() = default;

	void init() override;

	void update() override;

	void attach(Entity *entity);

	bool isAttached() { return attached; }

	void detach();

	void shoot();

	void setForce(float f);
};

} // namespace FishEngine