#include "../../include/fish_game/ECS/KeyboardController.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/SpriteComponent.hpp"

namespace FishEngine {

void KeyboardController::init() {
  move = &entity->getComponent<MoveComponent>();
  sprite = &entity->getComponent<SpriteComponent>();
}

void KeyboardController::update() {

  // move the player
  if (Game::game_event.type == SDL_KEYDOWN) {
    switch (Game::game_event.key.keysym.sym) {
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
      sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
      break;
    // right
    case SDLK_d:
      move->right();
      // sprite->play("swim");
      break;
    }
  }

  // stop the player
  if (Game::game_event.type == SDL_KEYUP) {
    switch (Game::game_event.key.keysym.sym) {
    // upwards
    case SDLK_w || SDLK_s || SDLK_d:
      move->stop();
      // sprite->play("idle");
      break;
    case SDLK_a:
      move->stop();
      // sprite->play("idle");
      sprite->spriteFlip = SDL_FLIP_NONE;
      break;
    case SDLK_ESCAPE:
      // TODO: add a pause menu
      // Game::stop();
      break;
    }
  }
}

} // namespace FishEngine