#include "../../include/fish_game/ECS/KeyboardController.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/SpriteComponent.hpp"

namespace FishEngine {

void KeyboardController::init() {
	std::cout << "KEYBOARDCONTROLLER INITIALIZED" << std::endl;
	move = &entity->getComponent<MoveComponent>();
	sprite = &entity->getComponent<SpriteComponent>();
}

void KeyboardController::update()

{
	if (Game::game_event.type == SDL_KEYDOWN) {
		std::cout << Game::game_event.key.keysym.sym << std::endl;
		switch (Game::game_event.key.keysym.sym) {
		// upwardsw
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
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
			// sprite->play("swim");
			break;
		}
	}

	// stop the player
	if (Game::game_event.type == SDL_KEYUP) {
		switch (Game::game_event.key.keysym.sym) {

		case SDLK_ESCAPE:
			// TODO: add a pause menu
			// Game::stop();
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
			sprite->spriteFlip = SDL_FLIP_NONE;

			// sprite->play("idle");
			break;
		}
	}
}

} // namespace FishEngine