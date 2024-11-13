#include "../../include/fish_game/ECS/ColliderComponent.hpp"
// #include "../../include/fish_game/ECS/ECS.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/ClientGame.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace FishEngine {

// #include "../include/fish_game/ClientGame.hpp"

ColliderComponent::ColliderComponent(std::string t, int xpos, int ypos, int xsize, int ysize) : tag(t) {
	collider.x = xpos;
	collider.y = ypos;
	collider.w = xsize;
	collider.h = ysize;
}

void ColliderComponent::init() {
	if (entity->hasComponent<TransformComponent>()) {
		transform = &entity->getComponent<TransformComponent>();
	} else {
		transform = &entity->getComponent<MoveComponent>();
	}
	// TODO: create a collider texture
	// tex = TextureManager::loadTexture("assets/coltex.png");

	// // TODO: read the size of the collider from the texture
	// srcRect = {0, 0, 16, 16};
	// dstRect = {collider.x, collider.y, collider.w, collider.h};
}

// TODO: adapt code to use tileson and different layers
void ColliderComponent::update() {
	collider.x = static_cast<int>(transform->position.getX());
	collider.y = static_cast<int>(transform->position.getY());
	collider.w = transform->width * transform->scale;
	collider.h = transform->height * transform->scale;

	// std::cout << "ColliderComponent - new pos: " << collider.x << " " << collider.y << std::endl;

	// dstRect.x = collider.x - ClientGame::camera.x;
	// dstRect.y = collider.y - ClientGame::camera.y;
}

void ColliderComponent::draw() {
	// todo: create a collider texture
	// TextureManager::draw(tex, srcRect, dstRect, SDL_FLIP_NONE);
}

} // namespace FishEngine