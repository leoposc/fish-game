#pragma once

#include "ECS.hpp"

#include <vector>

namespace FishEngine {

class ColliderComponent;
class WearableComponent;

class EquipmentComponent : public Component {

  private:
	bool isEquipped = false;
	mutable bool needsUpdate = false;
	uint8_t equippedID = -1;
	// std::shared_ptr<WearableComponent> wearable = nullptr;
	WearableComponent *wearable = nullptr;
	ColliderComponent *collider = nullptr;

  public:
	template <class Archive>
	void save(Archive &ar) const {
		ar(needsUpdate);
		if (needsUpdate) {
			ar(isEquipped, equippedID);
			needsUpdate = false;
		}
	}

	template <class Archive>
	void load(Archive &ar) {
		ar(needsUpdate);
		if (needsUpdate) {
			needsUpdate = false;
			ar(isEquipped, equippedID);
			if (isEquipped) {
				assert(equippedID != -1);
				equip(&entity->getManager()->getEntity(equippedID));
			} else {
				unequip();
			}
		}
	}

	EquipmentComponent() = default;
	~EquipmentComponent() = default;

	void init() override;

	void update() override;

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
