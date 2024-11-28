#include "../../include/fish_game/ECS/ComponentsGenerator.hpp"

namespace FishEngine {

namespace ClientGenerator {

void forPlayer(Entity &player, std::uint16_t x, std::uint16_t y) {
	player.addComponent<TransformComponent>(x, y, 45, 60, 1.0);
	player.addComponent<SpriteComponent>("fish", false);
	player.addComponent<ClientComponent>();
	player.addComponent<ColliderComponent>("player", x, y, 45, 60);
	player.addComponent<MoveComponent>();
	player.addComponent<GravityComponent>();
	player.addComponent<EquipmentComponent>();
	player.addComponent<EventHandlerComponent>();
	player.addGroup(ClientGame::groupLabels::groupPlayers);
}

void forEnemy(Entity &enemy, std::uint16_t x, std::uint16_t y) {
	enemy.addComponent<TransformComponent>(x, y, 45, 60, 1.0);
	enemy.addComponent<SpriteComponent>("fish", false);
	enemy.addComponent<ClientComponent>();
	enemy.addComponent<ColliderComponent>("enemy", x, y, 45, 60);
	enemy.addComponent<MoveComponent>();
	enemy.addComponent<GravityComponent>();
	enemy.addGroup(ClientGame::groupLabels::groupEnemies);
	enemy.addGroup(ClientGame::groupLabels::groupPlayers);
}

void forWeapon(Entity &weapon, std::uint16_t x, std::uint16_t y) {
	weapon.addComponent<TransformComponent>(x, y, 13, 18, 1.0);
	weapon.addComponent<SpriteComponent>("pistol", false);
	weapon.addComponent<ColliderComponent>("weapon", x, y, 13, 18);
	weapon.addComponent<WearableComponent>();
	weapon.addGroup(ClientGame::groupLabels::groupWeapons);
}

void forProjectile(Entity &projectile, std::uint16_t x, std::uint16_t) {}

} // namespace ClientGenerator

namespace ServerGenerator {

void forPlayer(Entity &player, std::uint16_t x, std::uint16_t y) {
	player.addComponent<TransformComponent>(x, y, 45, 60, 1.0);
	player.addComponent<ColliderComponent>("player", x, y, 45, 60);
	player.addComponent<MoveComponent>();
	player.addComponent<GravityComponent>();
	player.addComponent<EquipmentComponent>();
	player.addComponent<EventHandlerComponent>();
	player.addComponent<ServerComponent>();
	player.addGroup(ServerGame::groupLabels::groupPlayers);
}

void forProjectile(Entity &projectile, std::uint16_t x, std::uint16_t) {}

} // namespace ServerGenerator

} // namespace FishEngine