#include "../../include/fish_game/ECS/SpriteComponent.hpp"
#include "../../include/fish_game/ECS/ECS.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/Game.hpp"

// #include "Components.hpp"

// // #include "../AssetManager.hpp"
// #include "../Game.hpp"
// // // #include "ECS.hpp"

// #include "../TextureManager.hpp"
// // // #include "TransformComponent.hpp"

#include <SDL2/SDL.h>
#include <filesystem>
#include <map>
#include <string>

namespace FishEngine {

SpriteComponent::SpriteComponent(std::string id) : id(id) { setTexture(id); }

SpriteComponent::SpriteComponent(std::string id, bool isAnimated)
    : animated(isAnimated), id(id) {

  if (animated) {
    // Animation idle = Animation(0, 3, 100);
    // Animation walk = Animation(1, 8, 100);

    // animations.emplace("Idle", idle);
    // animations.emplace("Walk", walk);

    // play("Idle");
  }

  setTexture(id);
  std::cout << "SpriteComponent: " << id << " created!" << std::endl;
}

SpriteComponent::~SpriteComponent() { SDL_DestroyTexture(texture); }

void SpriteComponent::setTexture(std::string id) {
  texture = Game::assets->getTexture(id);
}

void SpriteComponent::init() {
  if (id == "fish") {
    if (entity->hasComponent<MoveComponent>()) {
      transform = &entity->getComponent<MoveComponent>();
      std::cout << "SpriteComponent: MoveComponent found." << std::endl;
    } else {
      std::cout << "SpriteComponent: no MoveComponent found." << std::endl;
    }
  } else {
    if (entity->hasComponent<TransformComponent>()) {
      transform = &entity->getComponent<TransformComponent>();
    } else {
      std::cout << "SpriteComponent: no TransformComponent found." << std::endl;
    }
  }

  srcRect.x = srcRect.y = 0;
  srcRect.w = static_cast<int>(transform->width * transform->scale);
  srcRect.h = static_cast<int>(transform->height * transform->scale);
}

void SpriteComponent::update() {
  // std::cout << "SpriteComponent: updating " << id << std::endl;
  // std::cout << "SpriteComponent: transform->getX() " << transform->getX()
  //           << std::endl;
  // std::cout << "SpriteComponent: transform->getY() " << transform->getY()
  //           << std::endl;
  dstRect.x = (int)transform->getX();
  dstRect.y = (int)transform->getY();
  dstRect.w = transform->width;
  dstRect.h = transform->height;
}

void SpriteComponent::draw() {
  // std::cout << "SpriteComponent: drawing " << id << "\n at " << dstRect.x
  //           << ", " << dstRect.y << "\n"
  //           << " with scale " << transform->scale << std::endl;
  TextureManager::draw(texture, srcRect, dstRect, spriteFlip);
}

// TODO: implement animation with tiled/ tileson
//   void play(const char *animationName) {
//     frames = animations[animationName].frames;
//     speed = animations[animationName].speed;
//     animated = true;
//   }

} // namespace FishEngine