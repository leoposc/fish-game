#include "../../include/fish_game/ECS/ColliderComponent.hpp"
#include "../../include/fish_game/ClientGame.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/ServerGame.hpp"

#include "spdlog/spdlog.h"
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
	// spdlog::get("console")->debug( "COLLIDER COMPONENT INITS" );
	transform = &entity->getComponent<TransformComponent>();
	lastPosition = transform->position;
	collider.x = static_cast<int>(transform->position.getX());
	collider.y = static_cast<int>(transform->position.getY());
	collider.w = transform->width * transform->scale;
	collider.h = transform->height * transform->scale;
}

void ColliderComponent::update() {

	if (ServerGame::checkCollisions(entity)) {
		SDL_Rect *collider = &entity->getComponent<ColliderComponent>().collider;
		transform->position = {static_cast<float>(collider->x), static_cast<float>(collider->y)};

		spdlog::get("console")->debug("ColliderComponent - collision detected");
	}

	collider.x = static_cast<int>(transform->position.getX());
	collider.y = static_cast<int>(transform->position.getY());
	collider.w = transform->width * transform->scale;
	collider.h = transform->height * transform->scale;

	// spdlog::get("console")->debug( "ColliderComponent - new pos: " << collider.x << " " << collider.y )
	lastPosition = transform->position;
}

void ColliderComponent::draw() {
	SDL_SetRenderDrawColor(ClientGame::renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(ClientGame::renderer, &collider);
	SDL_SetRenderDrawColor(ClientGame::renderer, 0, 0, 0, 255);
}

} // namespace FishEngine

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::ColliderComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::ColliderComponent)