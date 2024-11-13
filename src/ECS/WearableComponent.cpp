#include "../../include/fish_game/ECS/WearableComponent.hpp"
#include "../../include/fish_game/ECS/ColliderComponent.hpp"
#include "../../include/fish_game/ECS/MoveComponent.hpp"
#include "../../include/fish_game/ECS/ProjectileComponent.hpp"
#include "../../include/fish_game/ECS/SpriteComponent.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
// #include "../../include/fish_game/ECS/ECS.hpp"

namespace FishEngine {

// WearableComponent::WearableComponent(/* args */) {}

// WearableComponent::~WearableComponent() {}

void WearableComponent::init() {
	if (!entity->hasComponent<TransformComponent>()) {
		entity->addComponent<TransformComponent>();
	}
	transform = &entity->getComponent<TransformComponent>();
}

void WearableComponent::update() {
	if (isAttached) {
		// copy the values of the attached entity to the entity
		transform->position = attachedEntity->getComponent<MoveComponent>().position;
	}
}

void WearableComponent::attach(Entity *entity) {
	std::cout << "WearableComponent - attach" << std::endl;
	attachedEntity = entity;
	isAttached = true;
	// transform = &attachedEntity->getComponent<MoveComponent>();
	// std::cout << "Attached entity: " << transform << std::endl;
}

void WearableComponent::detach() {
	std::cout << "WearableComponent - detach" << std::endl;
	attachedEntity = nullptr;
	isAttached = false;
	// TransformComponent *tmp = transform;
	// transform = &entity->getComponent<TransformComponent>();
	// // copy the values of the attached entity to the entity
	// // TODO: PROBLEM: the values are not copied, but the pointer is copied
	// // create a deep copy of the transform component
	// *transform = *tmp;
}

void WearableComponent::shoot() {
	std::cout << "WearableComponent - shoot" << std::endl;
	// get manager
	auto man = entity->getManager();
	// create a new projectile
	auto &projectile(man->addEntity());
	// auto &projectile(this->entity->getManager()->addEntity());
	// projectile.addComponent<TransformComponent>(transform->position.getX(), transform->position.getY(), 16, 16, 1);
	// projectile.addComponent<ProjectileComponent>(500, 2, Vector2D(2, 0));
	// projectile.addComponent<ColliderComponent>("projectile");
	// projectile.addComponent<SpriteComponent>("projectile");
	// projectile.addGroup(Game::groupProjectiles);
	std::cout << "Projectile created, addr: " << &projectile << std::endl;
	// projectile.destroy();
}

} // namespace FishEngine