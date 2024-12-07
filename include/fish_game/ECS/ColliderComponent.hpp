#pragma once

// #include "Components.hpp"
#include "../Auxiliary.hpp"
#include "../ClientGame.hpp"
#include "ECS.hpp"

#include <SDL2/SDL.h>
#include <string>

namespace FishEngine {

class TransformComponent;

class ColliderComponent : public Component {
	Vector2D lastPosition{0, 0};

  public:
	template <class Archive>
	void serialize(Archive &ar) {
		ar(collider);
	}

	std::string tag;

	SDL_Rect collider;
	SDL_Texture *tex;

	TransformComponent *transform;

	ColliderComponent() = default;

	ColliderComponent(std::string t) : tag(t) {}

	ColliderComponent(std::string t, int xpos, int ypos, int xsize, int ysize);

	void init() override;

	void update() override;

	void draw() override;
};

// // Register the derived class with Cereal for polymorphic serialization, not sure if we need it
// CEREAL_REGISTER_TYPE(ColliderComponent)
// CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ColliderComponent)

} // namespace FishEngine