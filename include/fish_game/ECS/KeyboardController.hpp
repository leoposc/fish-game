#pragma once

#include "../Game.hpp"
#include "ECS.hpp"
#include "EquipmentComponent.hpp"

// #include "MoveComponent.hpp"
// #include "SpriteComponent.hpp"

#include <SDL2/SDL.h>

namespace FishEngine {

class SpriteComponent;
class MoveComponent;
class EquipmentComponent;

class KeyboardController : public Component {
	MoveComponent *move = nullptr;
	SpriteComponent *sprite = nullptr;
	EquipmentComponent *equip = nullptr;

  public:
	KeyboardController() = default;
	~KeyboardController() = default;

	void init() override;

	void update() override;
};

} // namespace FishEngine