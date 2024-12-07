#pragma once

#include "ECS.hpp"
// #include "ColliderComponent.hpp"
// #include "WearableComponent.hpp"

#include <vector>

namespace FishEngine {

class ColliderComponent;
class WearableComponent;

class EquipmentComponent : public Component {

  private:
	bool isEquipped = false;
	std::unique_ptr<WearableComponent> wearable;
	// WearableComponent *wearable = nullptr;
	ColliderComponent *collider = nullptr;

  public:
	template <class Archive>
	void serialize(Archive &ar) {
		ar(isEquipped, wearable);
	}

	EquipmentComponent() = default;
	~EquipmentComponent() = default;

	void init() override;

	void update() override {}

	/**
	 * @brief: pick up or drop the item
	 * @param: Entity *entity   the item to pick up. It can be nullptr when already equipped
	 *                          or an already attached Entity should be dropped
	 */
	void processCommand();

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

	/**
	 * @brief: shoot if equipped
	 */
	void shoot();
};

} // namespace FishEngine