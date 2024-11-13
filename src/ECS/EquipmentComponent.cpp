#include "../../include/fish_game/ECS/Equipment.hpp"

namespace FishEngine {

void EquipmentComponent::init() {
	std::cout << "EQUIPMENT COMPONENT INITIALIZED" << std::endl;
}

// void EquipmentComponent::update() {
// 	if (isEquipped) {
// 		std::cout << "EquipmentComponent - equipped" << std::endl;
// 	} else {
// 		std::cout << "EquipmentComponent - not equipped" << std::endl;
// 	}
// }

void EquipmentComponent::processCommand(Entity *entity) {
	if (isEquipped) {
		unequip();
	} else if (entity != nullptr) {
		equip(entity);
	}
}

void EquipmentComponent::equip(Entity *entity) {
	isEquipped = true;
	wearable = &entity->getComponent<WearableComponent>();
	assert(wearable != nullptr);
	wearable->attach(entity);
}

void EquipmentComponent::unequip() {
	isEquipped = false;
	if (wearable != nullptr) {
		wearable->detach();
	} else {
		std::cout << "No wearable attached!" << std::endl;
	}
}

} // namespace FishEngine