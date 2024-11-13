#pragma once

#include "ECS.hpp"

// #include "Components.hpp"

// // #include "../AssetManager.hpp"
// #include "../Game.hpp"
// // // #include "ECS.hpp"

// #include "../TextureManager.hpp"
#include "TransformComponent.hpp"

#include <SDL2/SDL.h>
#include <filesystem>
#include <map>
#include <string>

namespace FishEngine {

class SpriteComponent : public Component {
  TransformComponent *transform;
  SDL_Texture *texture;
  SDL_Rect srcRect, dstRect;

  bool animated = false;
  int frames = 0;
  int speed = 100; // ms

  std::string id;

public:
  int animationIndex = 0;
  // std::map<const char *, Animation> animations;

  SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

  SpriteComponent() = default;

  SpriteComponent(std::string id);

  SpriteComponent(std::string id, bool isAnimated);

  ~SpriteComponent();

  void setTexture(std::string id);

  void init() override;

  void update() override;

  void draw() override;
  // TODO: implement animation with tiled/ tileson
  //   void play(const char *animationName) {
  //     frames = animations[animationName].frames;
  //     speed = animations[animationName].speed;
  //     animated = true;
  //   }
};

} // namespace FishEngine