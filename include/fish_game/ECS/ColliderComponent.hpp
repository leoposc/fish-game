#pragma once

// #include "Components.hpp"
#include "../ClientGame.hpp"
#include "ECS.hpp"

#include <SDL2/SDL.h>
#include <string>

namespace FishEngine {

class TransformComponent;

class ColliderComponent : public Component {
  public:
	std::string tag;

	SDL_Rect collider;
	SDL_Texture *tex;
	SDL_Rect srcRect, dstRect;

	TransformComponent *transform;

	ColliderComponent(std::string t) : tag(t) {}

	ColliderComponent(std::string t, int xpos, int ypos, int xsize, int ysize);

	void init() override;

	// TODO: adapt code to use tileson and different layers
	void update() override;

	void draw() override;
};

} // namespace FishEngine