#include "../../include/fish_game/ECS/EquipmentComponent.hpp"
#include "../../include/fish_game/ECS/ColliderComponent.hpp"
#include "../../include/fish_game/ECS/ECS.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/WearableComponent.hpp"
#include "../../include/fish_game/ServerGame.hpp"

namespace FishEngine {

void EquipmentComponent::init() {
	std::cout << "EQUIPMENT COMPONENT INIT" << std::endl;
	collider = &entity->getComponent<ColliderComponent>();
}

// void EquipmentComponent::update() {
// 	if (isEquipped) {
// 		std::cout << "EquipmentComponent - equipped" << std::endl;
// 	} else {
// 		std::cout << "EquipmentComponent - not equipped" << std::endl;
// 	}
// }

void EquipmentComponent::processCommand() {
	if (isEquipped) {
		unequip();
	} else {
		Manager *manager = entity->getManager();
		std::vector<Entity *> weapons = manager->getGroup(ClientGame::groupWeapons);
		SDL_Rect col = collider->collider;
		for (auto weapon : weapons) {
			if (SDL_HasIntersection(&col, &weapon->getComponent<ColliderComponent>().collider)) {
				equip(weapon);
				break;
			}
		}
	}
}

void EquipmentComponent::equip(Entity *entity) {
	isEquipped = true;
	wearable = &entity->getComponent<WearableComponent>();
	assert(wearable != nullptr);
	wearable->attach(this->entity);
}

void EquipmentComponent::unequip() {
	isEquipped = false;
	if (wearable != nullptr) {
		wearable->detach();
	} else {
		std::cout << "No wearable attached!" << std::endl;
	}
}

void EquipmentComponent::shoot() {
	if (isEquipped) {
		wearable->shoot();
	} else {
		std::cout << "No weapon equipped!" << std::endl;
	}
}

} // namespace FishEngine