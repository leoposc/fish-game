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
	setTexture(id);
}

SpriteComponent::~SpriteComponent() {
	// SDL_DestroyTexture(texture);
}

void SpriteComponent::setTexture(std::string id) {
	texture = ClientGame::assets->getTexture(id);
}

void SpriteComponent::init() {
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

	if (animated) {
		currentFrame = (SDL_GetTicks() / speed) % frames;
		srcRect.x = currentFrame * srcRect.w;
		srcRect.y = animationIndex * transform->height;
		// spdlog::get("console")->debug("SpriteComponent: {} {} {}", id, currentFrame, srcRect.x);
	} else {
		srcRect.x = 0;
		srcRect.y = 0;
	}
	spriteFlip = transform->faceRight ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
}

void SpriteComponent::draw() {
	dstRect.x = (int)transform->getX();
	dstRect.y = (int)transform->getY();
	dstRect.w = transform->width;
	dstRect.h = transform->height;
	TextureManager::draw(texture, srcRect, dstRect, spriteFlip);
}

void SpriteComponent::play(const char *animationName) {
	animationIndex = animations[animationName].index;
	frames = animations[animationName].frames;
	speed = animations[animationName].speed;
}

} // namespace FishEngine

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(FishEngine::SpriteComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::SpriteComponent);