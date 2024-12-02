#pragma once

#include "ECS.hpp"

namespace FishEngine {

class TransformComponent;

class ServerComponent : public Component {

	TransformComponent *transform = nullptr;

  public:
	template <class Archive>
	void serialize(Archive &ar) {}

	ServerComponent() = default;
	~ServerComponent() = default;

	void init() override;

	void update() override;
};

} // namespace FishEngine