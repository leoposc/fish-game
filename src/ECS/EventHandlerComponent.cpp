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
			if (isServer) {
				if (event_ptr->key.keysym.sym == SDLK_j) {
					equip->processCommand();
				}
				if (event_ptr->key.keysym.sym == SDLK_k) {
					spdlog::get("console")->info("Eventhandler: Shooting");
					equip->shoot();
				}
			}

			// send the event to the server
			if (!isServer) {
				spdlog::get("network_logger")->debug("Sending event to server {}", event_ptr->key.keysym.sym);
				clientComponent->sendEvent(*event_ptr);
			} else {
				// reset the event - otherwise the event will be processed multiple times
				// on the server side, since the server does not poll and therefore override
				// the event
				serverComponent->setEvent(SDL_Event());
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
			} else {
				// reset the event - otherwise the event will be processed multiple times
				// on the server side, since the server does not poll and therefore override
				// the event
				serverComponent->setEvent(SDL_Event());
			}
		}
	}
}

} // namespace FishEngine
