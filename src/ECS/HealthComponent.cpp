#include "../../include/fish_game/ECS/HealthComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>
#include <spdlog/spdlog.h>

CEREAL_REGISTER_TYPE(FishEngine::HealthComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::HealthComponent);

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

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

	if (!alive && SDL_GetTicks() - startTimer > 3000) {

		spdlog::get("console")->debug("HealthComponent - entity {} destroyed, not alive anymore", entity->getID());
		this->entity->destroy();
	} else if ((transform->getY() > SCREEN_HEIGHT) || (transform->getY() < 0) || (transform->getX() > SCREEN_WIDTH)
	           || (transform->getX() < 0)) {

		// spdlog::get("console")->debug("HealthComponent - entity {} moved out of screen. Position at: {} {}",
		//                               entity->getID(), transform->position.getX(), transform->position.getY());
		takeDamage();
	}
}

} // namespace FishEngine
