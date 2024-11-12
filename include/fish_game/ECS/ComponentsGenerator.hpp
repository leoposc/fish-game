#pragma once

#include "ColliderComponent.hpp"
#include "ECS.hpp"
#include "EventHandlerComponent.hpp"
#include "MoveComponent.hpp"
#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"

namespace FishEngine {
class ClientComponentsGenerator {
  public:
	static void forPlayer(Entity &player, std::uint16_t x, std::uint16_t y) {
		player.addComponent<ClientTransformComponent>(x, y, 45, 60, 1.0);
		player.addComponent<SpriteComponent>("fish", false);
		player.addGroup(ClientGame::groupLabels::groupPlayers);
	}

	static void forProjectile(Entity &projectile, std::uint16_t x, std::uint16_t) {}
};

class ServerComponentsGenerator {
  public:
	static void forPlayer(Entity &player, std::uint16_t x, std::uint16_t y) {
		player.addComponent<MoveComponent>(x, y, 45, 60, 1.0);
		player.addComponent<ColliderComponent>("player", x, y, 45, 60);
		player.addComponent<EventHandlerComponent>();
		player.addGroup(ServerGame::groupLabels::groupPlayers);
	}

	static void forProjectile(Entity &projectile, std::uint16_t x, std::uint16_t) {}
};
} // namespace FishEngine