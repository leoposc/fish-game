#include "../../include/fish_game/ECS/HealthComponent.hpp"
#include "../../include/fish_game/ECS/SpriteComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>
#include <spdlog/spdlog.h>

CEREAL_REGISTER_TYPE(FishEngine::HealthComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::HealthComponent);

constexpr int SCREEN_WIDTH = 2048;
constexpr int SCREEN_HEIGHT = 1024;

namespace FishEngine {

void HealthComponent::init() {
	transform = &entity->getComponent<TransformComponent>();
}

void HealthComponent::takeDamage() {
	if (alive) {
		alive = false;
		startTimer = SDL_GetTicks();
	}
}

void HealthComponent::update() {

	// case when the entity is not alive anymore and the timer has expired
	if (!alive && SDL_GetTicks() - startTimer > 3000) {

		spdlog::get("console")->debug("HealthComponent - entity {} destroyed, not alive anymore", entity->getID());
		this->entity->destroy();

		// case when the entity is not alive anymore and the timer has not expired

	} else if (transform->getY() > SCREEN_HEIGHT + 50 || (transform->getX() > SCREEN_WIDTH + 50)
	           || (transform->getX() < -50)) {

		takeDamage();

	} else if (!alive && entity->hasComponent<SpriteComponent>()) {

		entity->getComponent<SpriteComponent>().play("fishDead");
	}
}

} // namespace FishEngine
