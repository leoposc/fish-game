#include "../../include/fish_game/ECS/EventHandlerComponent.hpp"
#include "../../include/fish_game/ClientGame.hpp"
#include "../../include/fish_game/ECS/ClientComponent.hpp"
#include "../../include/fish_game/ECS/EquipmentComponent.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/ServerComponent.hpp"

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
}

void EventHandlerComponent::update()

{
	if (event_ptr->type == SDL_KEYDOWN) {
		switch (event_ptr->key.keysym.sym) {
		// upwards
		case SDLK_w:
			move->up();
			// sprite->play("swim");
			break;
		// downwards
		case SDLK_s:
			move->down();
			// sprite->play("swim");
			break;
		// left
		case SDLK_a:
			move->left();
			// sprite->play("swim");
			break;
		// right
		case SDLK_d:
			move->right();
			// sprite->play("swim");
			break;
		// equip/ unequip
		case SDLK_j:
			spdlog::get("console")->debug("J pressed");
			equip->processCommand();
			break;
		case SDLK_k:
			equip->shoot();
			break;
		}

		// send the event to the server
		if (!isServer) {
			clientComponent->sendEvent(ClientGame::game_event);
		}
	}

	// stop the player
	if (event_ptr->type == SDL_KEYUP) {
		switch (event_ptr->key.keysym.sym) {
		case SDLK_ESCAPE:
			// TODO: add a pause menu
			// ClientGame::stop();
			break;
			// upwards
		case SDLK_w:
			move->stop();
			// sprite->play("idle");
			break;
		// downwards
		case SDLK_s:
			move->stop();
			// sprite->play("idle");
			break;
		// left
		case SDLK_a:
			move->stop();
			// sprite->play("idle");
			// sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
			break;
		// right
		case SDLK_d:
			move->stop();
			// sprite->play("idle");
			break;
		}

		// send the event to the server
		if (!isServer) {
			clientComponent->sendEvent(ClientGame::game_event);
		}
	}
	// spdlog::get("console")->debug("EVENTHANDLER COMPONENT UPDATED");

	//
}

} // namespace FishEngine

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::EventHandlerComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::EventHandlerComponent)