#pragma once

#include "ECS.hpp"

namespace FishEngine {

class TransformComponent;

class ClientComponent : public Component {

	TransformComponent *transform = nullptr;

  public:
	ClientComponent() = default;
	virtual ~ClientComponent() = default;

	void init() override;

	void update() override;
};

} // namespace FishEngine