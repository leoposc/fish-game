#pragma once

#include "ECS.hpp"
#include "WearableComponent.hpp"

namespace FishEngine {

class EquipmentComponent : public Component {

  private:
	bool isEquipped = false;
	WearableComponent *wearable = nullptr;

  public:
	EquipmentComponent() = default;
	~EquipmentComponent() = default;

	void init() override;

	void update() override {}

	/**
	 * @brief: pick up or drop the item
	 * @param: Entity *entity   the item to pick up. It can be nullptr when already equipped
	 *                          or an already attached Entity should be dropped
	 */
	void processCommand(Entity *entity);

	/**
	 * @brief: equip the item
	 * @details: function is automatically called by the processCommand function
	 */
	void equip(Entity *entity);

	/**
	 * @brief: unequip the item
	 * @details: function is automatically called by the processCommand function
	 */
	void unequip();
};

} // namespace FishEngine