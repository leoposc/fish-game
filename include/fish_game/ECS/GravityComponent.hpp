#pragma once

#include "ECS.hpp"
#include "MoveComponent.hpp"
#include "TransformComponent.hpp"

namespace FishEngine {

class GravityComponent : public Component {

	TransformComponent *transform = nullptr;
	MoveComponent *move = nullptr;

  public:
	template <class Archive>
	void serialize(Archive &ar) {}

	GravityComponent() = default;
	~GravityComponent() = default;

	void init() override;

	void update() override;
};

} // namespace FishEngine