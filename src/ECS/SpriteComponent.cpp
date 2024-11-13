#include "../../include/fish_game/ECS/SpriteComponent.hpp"
#include "../../include/fish_game/ECS/ECS.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/Game.hpp"

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
	std::cout << "SpriteComponent: " << id << " created!" << std::endl;
}

SpriteComponent::~SpriteComponent() {
	SDL_DestroyTexture(texture);
}

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
	if (id == "pistol") {
		std::cout << "SpriteComponent: updating " << id << std::endl;
	}
	dstRect.x = (int)transform->getX();
	dstRect.y = (int)transform->getY();
	dstRect.w = transform->width;
	dstRect.h = transform->height;
}

void SpriteComponent::draw() {
	TextureManager::draw(texture, srcRect, dstRect, spriteFlip);
}

} // namespace FishEngine