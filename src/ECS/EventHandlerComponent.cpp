#include "../../include/fish_game/ECS/EventHandlerComponent.hpp"
#include "../../include/fish_game/ClientGame.hpp"
#include "../../include/fish_game/ECS/EquipmentComponent.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"

namespace FishEngine {

void EventHandlerComponent::init() {
	std::cout << "EVENT HANDLER COMPONENT INTI" << std::endl;
	if (!entity->hasComponent<MoveComponent>()) {
		std::cout << "Event Handler Component: no MoveComponent found." << std::endl;
	} else {
		move = &entity->getComponent<MoveComponent>();
	}
	if (!entity->hasComponent<EquipmentComponent>()) {
		std::cout << "Event Handler Component: no EquipmentComponent found." << std::endl;
	} else {
		equip = &entity->getComponent<EquipmentComponent>();
	}
}

void EventHandlerComponent::update()

{
	if (ClientGame::game_event.type == SDL_KEYDOWN) {
		switch (ClientGame::game_event.key.keysym.sym) {
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
			equip->processCommand();
			break;
		case SDLK_k:
			equip->shoot();
			break;
		}
	}

	// stop the player
	if (ClientGame::game_event.type == SDL_KEYUP) {
		switch (ClientGame::game_event.key.keysym.sym) {
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
	}
	std::cout << "EVENTHANDLER COMPONENT UPDATED" << std::endl;
}

} // namespace FishEngine