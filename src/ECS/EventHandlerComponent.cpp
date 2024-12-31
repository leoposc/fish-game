#include "../../include/fish_game/ECS/EventHandlerComponent.hpp"
#include "../../include/fish_game/ClientGame.hpp"
#include "../../include/fish_game/ECS/ClientComponent.hpp"
#include "../../include/fish_game/ECS/EquipmentComponent.hpp"
#include "../../include/fish_game/ECS/HealthComponent.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/ServerComponent.hpp"

#include "fish_game/ServerGame.hpp"
#include "spdlog/spdlog.h"

namespace FishEngine {

void EventHandlerComponent::init() {
	spdlog::get("console")->debug("EVENT HANDLER COMPONENT INTI");
	if (!entity->hasComponent<MoveComponent>()) {
		spdlog::get("console")->debug("Event Handler Component: no MoveComponent found.");
	} else {
		move = &entity->getComponent<MoveComponent>();
	}
	if (!entity->hasComponent<EquipmentComponent>()) {
		spdlog::get("console")->debug("Event Handler Component: no EquipmentComponent found.");
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

void EventHandlerComponent::update()

{
	if (health->isAlive()) {
		if (event_ptr->type == SDL_KEYDOWN) {

			if (event_ptr->key.keysym.sym == SDLK_w) {
				// upwards
				move->up();
				// sprite->play("swim");
			} else if (event_ptr->key.keysym.sym == SDLK_s) {
				// downwards
				move->down();
				// sprite->play("swim");
			} else if (event_ptr->key.keysym.sym == SDLK_p) {
				// downwards
				FishEngine::ServerGame::getInstance().networkHost.printEventQueue();
				// sprite->play("swim");
			}
			if (event_ptr->key.keysym.sym == SDLK_d) {
				// right
				move->right();
				// sprite->play("swim");
			} else if (event_ptr->key.keysym.sym == SDLK_a) {
				// left
				move->left();
				// sprite->play("swim");
			}
			if (event_ptr->key.keysym.sym == SDLK_j) {
				// equip/ unequip
				spdlog::get("console")->debug("J pressed");
				equip->processCommand();
			}
			if (event_ptr->key.keysym.sym == SDLK_k) {
				// shoot
				equip->shoot();
			}

			// send the event to the server
			if (!isServer) {
				clientComponent->sendEvent(ClientGame::game_event);
			}
		}

		// stop the player
		if (event_ptr->type == SDL_KEYUP) {
			if (event_ptr->key.keysym.sym == SDLK_w) {
				// upwards
				move->stopY();
				// sprite->play("swim");
			} else if (event_ptr->key.keysym.sym == SDLK_s) {
				// downwards
				move->stopY();
				// sprite->play("swim");
			}
			if (event_ptr->key.keysym.sym == SDLK_d) {
				// right
				move->stopX();
				// sprite->play("swim");
			} else if (event_ptr->key.keysym.sym == SDLK_a) {
				// left
				move->stopX();
				// sprite->play("swim");
			}

			// send the event to the server
			if (!isServer) {
				clientComponent->sendEvent(ClientGame::game_event);
			}
		}
		// spdlog::get("console")->debug("EVENTHANDLER COMPONENT UPDATED");
	}
}

} // namespace FishEngine

#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::EventHandlerComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::EventHandlerComponent)
