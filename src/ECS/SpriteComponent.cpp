#include "../../include/fish_game/ECS/SpriteComponent.hpp"
#include "../../include/fish_game/ClientGame.hpp"
#include "../../include/fish_game/ECS/ECS.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"

#include "spdlog/spdlog.h"

#include <SDL2/SDL.h>
#include <filesystem>
#include <map>
#include <string>

namespace FishEngine {

SpriteComponent::SpriteComponent(std::string id) : id(id) {
	setTexture(id);
}

SpriteComponent::SpriteComponent(std::string id, bool isAnimated) : animated(isAnimated), id(id) {
	// TODO: implement animation with tiled/ tileson
	setTexture(id);
}

SpriteComponent::~SpriteComponent() {
	// SDL_DestroyTexture(texture);
}

void SpriteComponent::setTexture(std::string id) {
	texture = ClientGame::assets->getTexture(id);
}

void SpriteComponent::init() {
	// spdlog::get("console")->debug( "SpriteComponent: initializing " << id );
	if (entity->hasComponent<TransformComponent>()) {
		transform = &entity->getComponent<TransformComponent>();
	} else {
		spdlog::get("console")->debug("SpriteComponent: no ClientTransformComponent found.");
	}

	srcRect.x = srcRect.y = 0;
	srcRect.w = static_cast<int>(transform->width * transform->scale);
	srcRect.h = static_cast<int>(transform->height * transform->scale);
}

void SpriteComponent::update() {
	// spdlog::get("console")->debug("SpriteComponent: updating {}", id);
	// if (id == "pistol") {
	// 	spdlog::get("console")->debug( "SpriteComponent: updating " << id );
	// }
	dstRect.x = (int)transform->getX();
	dstRect.y = (int)transform->getY();
	dstRect.w = transform->width;
	dstRect.h = transform->height;
}

void SpriteComponent::draw() {
	TextureManager::draw(texture, srcRect, dstRect, spriteFlip);
}

} // namespace FishEngine

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::SpriteComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::SpriteComponent);