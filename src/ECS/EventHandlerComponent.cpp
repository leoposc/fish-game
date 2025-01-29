#include "../../include/fish_game/ECS/EventHandlerComponent.hpp"
#include "../../include/fish_game/ClientGame.hpp"
#include "../../include/fish_game/ECS/ClientComponent.hpp"
#include "../../include/fish_game/ECS/EquipmentComponent.hpp"
#include "../../include/fish_game/ECS/HealthComponent.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/ServerComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"

#include "fish_game/ServerGame.hpp"
#include "spdlog/spdlog.h"

namespace FishEngine {

void EventHandlerComponent::init() {
	spdlog::get("console")->debug("EVENT HANDLER COMPONENT INTI");
	if (!entity->hasComponent<MoveComponent>()) {
		spdlog::get("err_logger")->debug("Event Handler Component: no MoveComponent found.");
	} else {
		move = &entity->getComponent<MoveComponent>();
	}
	if (!entity->hasComponent<EquipmentComponent>()) {
		spdlog::get("err_logger")->debug("Event Handler Component: no EquipmentComponent found.");
	} else {
		equip = &entity->getComponent<EquipmentComponent>();
	}

	if (isServer) {
		serverComponent = &entity->getComponent<ServerComponent>();
		event_ptr = serverComponent->getEventPtr();
	} else {
		clientComponent = &entity->getComponent<ClientComponent>();
		event_ptr = &ClientGame::game_event;
	}

	health = &entity->getComponent<HealthComponent>();
}

void EventHandlerComponent::update() {

	if (health->isAlive()) {
		if (event_ptr->type == SDL_KEYDOWN) {

			if (event_ptr->key.keysym.sym == SDLK_w) {
				move->up();
			}
			if (event_ptr->key.keysym.sym == SDLK_s) {
				move->down();
			}
			if (event_ptr->key.keysym.sym == SDLK_a) {
				move->left();
			}
			if (event_ptr->key.keysym.sym == SDLK_d) {
				move->right();
			}
			if (event_ptr->key.keysym.sym == SDLK_j) {
				if (isServer)
					spdlog::get("console")->debug("Server: J pressed");
				equip->processCommand();
			}
			if (event_ptr->key.keysym.sym == SDLK_k) {
				equip->shoot();
				if (isServer)
					spdlog::get("console")->debug("Server: K pressed");
				equip->processCommand();
			}

			// send the event to the server
			if (!isServer) {
				spdlog::get("console")->debug("Sending event to server {}", event_ptr->key.keysym.sym);
				clientComponent->sendEvent(*event_ptr);
			}
		}

		// stop the player
		if (event_ptr->type == SDL_KEYUP) {
			if (event_ptr->key.keysym.sym == SDLK_w) {
				move->stopY();
			}
			if (event_ptr->key.keysym.sym == SDLK_s) {
				move->stopY();
			}
			if (event_ptr->key.keysym.sym == SDLK_a) {
				move->stopX();
			}
			if (event_ptr->key.keysym.sym == SDLK_d) {
				move->stopX();
			}

			// send the event to the server
			if (!isServer) {
				clientComponent->sendEvent(*event_ptr);
			}
		}
	}
}

} // namespace FishEngine

#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::EventHandlerComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::EventHandlerComponent)
