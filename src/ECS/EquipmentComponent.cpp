#include "../../include/fish_game/ECS/EquipmentComponent.hpp"
#include "../../include/fish_game/ECS/ColliderComponent.hpp"
#include "../../include/fish_game/ECS/ECS.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/WearableComponent.hpp"
#include "../../include/fish_game/Game.hpp"

namespace FishEngine {

void EquipmentComponent::init() {
	collider = &entity->getComponent<ColliderComponent>();
	std::cout << "EQUIPMENT COMPONENT INITIALIZED" << std::endl;
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
		std::vector<Entity *> weapons = manager->getGroup(Game::groupWeapons);
		SDL_Rect col = collider->collider;
		for (auto weapon : weapons) {
			std::cout << "Checking weapon" << std::endl;
			if (SDL_HasIntersection(&col, &weapon->getComponent<ColliderComponent>().collider)) {
				equip(weapon);
			}
		}
	}
}

void EquipmentComponent::equip(Entity *entity) {
	isEquipped = true;
	wearable = &entity->getComponent<WearableComponent>();
	assert(wearable != nullptr);
	wearable->attach(this->entity);
	std::cout << this->entity->hasComponent<MoveComponent>() << std::endl;
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