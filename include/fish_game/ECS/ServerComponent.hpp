#pragma once

#include "ECS.hpp"

namespace FishEngine {

class TransformComponent;

class ServerComponent : public Component {

	TransformComponent *transform = nullptr;

  public:
	ServerComponent() = default;
	~ServerComponent() = default;

	void init() override;

	void update() override;
};

} // namespace FishEngine