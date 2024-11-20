#pragma once

#include "ClientComponent.hpp"
#include "ColliderComponent.hpp"
#include "ECS.hpp"
#include "EventHandlerComponent.hpp"
#include "GravityComponent.hpp"
#include "MoveComponent.hpp"
#include "ServerComponent.hpp"
#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"

namespace FishEngine {
class ClientComponentsGenerator {
  public:
	static void forPlayer(Entity &player, std::uint16_t x, std::uint16_t y) {
		player.addComponent<TransformComponent>(x, y, 45, 60, 1.0);
		player.addComponent<SpriteComponent>("fish", false);
		player.addComponent<ClientComponent>();
		player.addGroup(ClientGame::groupLabels::groupPlayers);
	}

	static void forProjectile(Entity &projectile, std::uint16_t x, std::uint16_t) {}
};

class ServerComponentsGenerator {
  public:
	static void forPlayer(Entity &player, std::uint16_t x, std::uint16_t y) {
		player.addComponent<TransformComponent>(x, y, 45, 60, 1.0);
		player.addComponent<ColliderComponent>("player", x, y, 45, 60);
		player.addComponent<MoveComponent>();
		player.addComponent<GravityComponent>();
		player.addComponent<EquipmentComponent>();
		player.addComponent<EventHandlerComponent>();
		player.addComponent<ServerComponent>();
		player.addGroup(ServerGame::groupLabels::groupPlayers);
	}

	static void forProjectile(Entity &projectile, std::uint16_t x, std::uint16_t) {}
};
} // namespace FishEngine