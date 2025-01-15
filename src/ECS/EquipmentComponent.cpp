#include "../../include/fish_game/ECS/EquipmentComponent.hpp"
#include "../../include/fish_game/ECS/ColliderComponent.hpp"
#include "../../include/fish_game/ECS/ECS.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/WearableComponent.hpp"
#include "../../include/fish_game/ServerGame.hpp"

#include "spdlog/spdlog.h"

namespace FishEngine {

void EquipmentComponent::init() {
	// spdlog::get("console")->debug("EQUIPMENT COMPONENT INIT");
	collider = &entity->getComponent<ColliderComponent>();
}

void EquipmentComponent::processCommand() {
	if (isEquipped) {
		unequip();
	} else {
		Manager *manager = entity->getManager();
		std::vector<Entity *> weapons = manager->getGroup(ClientGame::groupWeapons);
		SDL_Rect col = collider->collider;
		for (auto weapon : weapons) {
			if (SDL_HasIntersection(&col, &weapon->getComponent<ColliderComponent>().collider)) {
				if (weapon->getComponent<WearableComponent>().isAttached()) {
					spdlog::info("Weapon already attached!");
					return;
				} else {
					spdlog::info("Weapon attached!");
					equip(weapon);
					break;
				}
			}
		}
	}
}

void EquipmentComponent::equip(Entity *entity) {
	isEquipped = true;
	wearable = entity->getComponentSmartPtr<WearableComponent>();
	// wearable = &entity->getComponent<WearableComponent>();

	// attach the weapon to the player
	assert(wearable != nullptr);
	wearable->attach(this->entity);
}

void EquipmentComponent::unequip() {
	isEquipped = false;
	if (wearable != nullptr) {
		wearable->detach();
	} else {
		spdlog::info("No wearable attached!");
	}
}

void EquipmentComponent::shoot() {
	if (isEquipped) {
		wearable->shoot();
	} else {
		spdlog::info("No weapon equipped!");
	}
}

} // namespace FishEngine

#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::EquipmentComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::EquipmentComponent)