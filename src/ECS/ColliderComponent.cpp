#include "../../include/fish_game/ECS/ColliderComponent.hpp"
#include "../../include/fish_game/ClientGame.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace FishEngine {

ColliderComponent::ColliderComponent(std::string t, int xpos, int ypos, int xsize, int ysize) : tag(t) {
	collider.x = xpos;
	collider.y = ypos;
	collider.w = xsize;
	collider.h = ysize;
}

void ColliderComponent::init() {
	transform = &entity->getComponent<TransformComponent>();
}

void ColliderComponent::update() {
	collider.x = static_cast<int>(transform->position.getX());
	collider.y = static_cast<int>(transform->position.getY());
	collider.w = transform->width * transform->scale;
	collider.h = transform->height * transform->scale;
	// std::cout << "ColliderComponent - new pos: " << collider.x << " " << collider.y << std::endl;
}

void ColliderComponent::draw() {
	SDL_SetRenderDrawColor(ClientGame::renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(ClientGame::renderer, &collider);
	SDL_SetRenderDrawColor(ClientGame::renderer, 0, 0, 0, 255);
}

} // namespace FishEngine