#include "../../include/fish_game/ECS/EquipmentComponent.hpp"
#include "../../include/fish_game/ECS/ColliderComponent.hpp"
#include "../../include/fish_game/ECS/ComponentsGenerator.hpp"
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
		std::vector<Entity *> weapons = manager->getGroup(groupWeapons);
		SDL_Rect col = collider->collider;
		// check if there is a weapon for the player to pick up
		for (auto weapon : weapons) {
			if (SDL_HasIntersection(&col, &weapon->getComponent<ColliderComponent>().collider)) {
				// check if the weapon is already attached
				if (weapon->getComponent<WearableComponent>().isAttached()) {
					spdlog::info("Weapon already attached!");
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
	needsUpdate = true;
	isEquipped = true;
	equippedID = entity->getID();
	wearable = entity->getComponentSmartPtr<WearableComponent>();
	// wearable = &entity->getComponent<WearableComponent>();

	// attach the weapon to the player
	assert(wearable != nullptr);
	wearable->attach(this->entity);
}

void EquipmentComponent::unequip() {
	needsUpdate = true;
	isEquipped = false;
	equippedID = -1;
	if (wearable != nullptr) {
		wearable->detach();
	} else {
		spdlog::get("console")->info("No wearable attached!");
	}
}

void EquipmentComponent::shoot() {
	spdlog::get("console")->info("EquipmentComponent::shoot() {}", isEquipped);
	if (isEquipped) {
		wearable->shoot();
	} else {
		spdlog::get("console")->info("No weapon equipped!");
	}
}

} // namespace FishEngine

#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::EquipmentComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::EquipmentComponent)