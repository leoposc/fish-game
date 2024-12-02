#pragma once

#include "../ServerGame.hpp"
#include "ECS.hpp"
#include "EquipmentComponent.hpp"

#include <SDL2/SDL.h>

namespace FishEngine {

class MoveComponent;

class EventHandlerComponent : public Component {

  public:
	template <class Archive>
	void serialize(Archive &ar) {}

	MoveComponent *move;
	EquipmentComponent *equip;

	EventHandlerComponent() = default;
	~EventHandlerComponent() = default;

	void init() override;

	void update() override;
};

} // namespace FishEngine