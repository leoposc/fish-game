#include "../include/fish_game/ECS/ComponentsGenerator.hpp"

namespace FishEngine {

namespace ClientGenerator {

void forPlayer(Entity &player, std::pair<std::uint16_t, std::uint16_t> const &pos) {
	player.addComponent<TransformComponent>(pos.first, pos.second, 45, 60, 1.0);
	player.addComponent<SpriteComponent>("fish", false);
	player.addComponent<ClientComponent>();
	player.addComponent<ColliderComponent>("player", pos.first, pos.second, 45, 60);
	player.addComponent<MoveComponent>();
	player.addComponent<GravityComponent>();
	player.addComponent<EquipmentComponent>();
	player.addComponent<EventHandlerComponent>(false);
	player.addComponent<HealthComponent>();
	player.addGroup(ClientGame::groupLabels::groupPlayers);
}

void forEnemy(Entity &enemy, std::pair<std::uint16_t, std::uint16_t> const &pos) {
	enemy.addComponent<TransformComponent>(pos.first, pos.second, 45, 60, 1.0);
	enemy.addComponent<SpriteComponent>("fish", false);
	// enemy.addComponent<ClientComponent>();
	enemy.addComponent<ColliderComponent>("enemy", pos.first, pos.second, 45, 60);
	enemy.addComponent<MoveComponent>();
	enemy.addComponent<GravityComponent>();
	enemy.addComponent<EquipmentComponent>();
	enemy.addComponent<HealthComponent>();
	enemy.addGroup(ClientGame::groupLabels::groupEnemies);
	enemy.addGroup(ClientGame::groupLabels::groupPlayers);
}

void forWeapon(Entity &weapon, std::pair<std::uint16_t, std::uint16_t> const &pos) {
	weapon.addComponent<TransformComponent>(pos.first, pos.second, 13, 18, 1.0);
	weapon.addComponent<SpriteComponent>("pistol", false);
	weapon.addComponent<ColliderComponent>("weapon", pos.first, pos.second, 13, 18);
	weapon.addComponent<WearableComponent>();
	weapon.addComponent<GravityComponent>();
	weapon.addGroup(ClientGame::groupLabels::groupWeapons);
}

void forProjectile(Entity &projectile, std::pair<std::uint16_t, std::uint16_t> const &pos) {
	projectile.addComponent<TransformComponent>(pos.first, pos.second, 5, 5, 1.0);
	projectile.addComponent<SpriteComponent>("projectile", false);
	projectile.addComponent<ColliderComponent>("projectile", pos.first, pos.second, 5, 5);
	Vector2D velocity = {-10, 0};
	projectile.addComponent<ProjectileComponent>(500, 10, velocity);
	projectile.addGroup(ClientGame::groupLabels::groupProjectiles);
}

} // namespace ClientGenerator

namespace ServerGenerator {

void forPlayer(Entity &player, std::pair<std::uint16_t, std::uint16_t> const &pos) {
	player.addComponent<TransformComponent>(pos.first, pos.second, 45, 60, 1.0);
	player.addComponent<ColliderComponent>("player", pos.first, pos.second, 45, 60);
	player.addComponent<MoveComponent>();
	player.addComponent<GravityComponent>();
	player.addComponent<ServerComponent>();
	player.addComponent<EquipmentComponent>();
	player.addComponent<EventHandlerComponent>(true);
	player.addComponent<HealthComponent>();
	player.addGroup(ServerGame::groupLabels::groupPlayers);
}

void forProjectile(Entity &projectile, std::pair<std::uint16_t, std::uint16_t> const &pos) {}

} // namespace ServerGenerator

} // namespace FishEngine