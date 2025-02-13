#include "../include/fish_game/ECS/ComponentsGenerator.hpp"
#include "../include/fish_game/MusicPlayer.hpp"

namespace FishEngine {

namespace ClientGenerator {

void forPlayer(Entity &player, std::pair<std::uint16_t, std::uint16_t> const &pos, size_t fishSpriteID) {
	player.addComponent<TransformComponent>(pos.first, pos.second, 30, 40, 1);
	player.addComponent<SpriteComponent>("fish0" + std::to_string(fishSpriteID), true);
	player.addComponent<ClientComponent>();
	player.addComponent<ColliderComponent>("player", pos.first, pos.second, 30, 40);
	player.addComponent<GravityComponent>();
	player.addComponent<MoveComponent, 0>();
	player.addComponent<EquipmentComponent>();
	player.addComponent<HealthComponent>();
	player.addComponent<EventHandlerComponent, 0>(false);
	player.addGroup(groupLabels::groupPlayers);
	player.addGroup(groupLabels::groupColliders);
}

void forEnemy(Entity &enemy, std::pair<std::uint16_t, std::uint16_t> const &pos, size_t fishSpriteID) {
	enemy.addComponent<TransformComponent>(pos.first, pos.second, 30, 40, 1.0);
	enemy.addComponent<SpriteComponent>("fish0" + std::to_string(fishSpriteID), true);
	enemy.addComponent<ColliderComponent>("enemy", pos.first, pos.second, 30, 40);
	enemy.addComponent<GravityComponent>();
	enemy.addComponent<MoveComponent, 0>();
	enemy.addComponent<EquipmentComponent>();
	enemy.addComponent<HealthComponent>();
	enemy.addGroup(groupLabels::groupEnemies);
	enemy.addGroup(groupLabels::groupPlayers);
	enemy.addGroup(groupLabels::groupColliders);
}

void forWeapon(Entity &weapon, std::pair<std::uint16_t, std::uint16_t> const &pos) {
	weapon.addComponent<TransformComponent>(pos.first, pos.second, 20, 16, 1.0);
	weapon.addComponent<SpriteComponent>("present", false);
	weapon.addComponent<ColliderComponent>("weapon", pos.first, pos.second, 13, 18);
	weapon.addComponent<WearableComponent>();
	weapon.addComponent<GravityComponent>();
	weapon.addGroup(groupLabels::groupWeapons);
	weapon.addGroup(groupLabels::groupColliders);
}

void forProjectile(Entity &projectile, std::pair<std::uint16_t, std::uint16_t> const &pos) {
	// velocity gets synced from server
	Vector2D velocity = Vector2D(0, 0);
	projectile.addComponent<TransformComponent>(pos.first, pos.second, 5, 5, 1.0);
	projectile.addComponent<SpriteComponent>("projectile", false);
	projectile.addComponent<ColliderComponent>("projectile", pos.first, pos.second, 5, 5);
	projectile.addComponent<ProjectileComponent>(500, 10, velocity);
	projectile.addGroup(groupLabels::groupProjectiles);
	projectile.addGroup(groupLabels::groupColliders);

	// hear shots from all players
	MusicPlayer::getInstance().playShootSound();
}

} // namespace ClientGenerator

namespace ServerGenerator {

void forPlayer(Entity &player, std::pair<std::uint16_t, std::uint16_t> const &pos) {
	player.addComponent<TransformComponent>(pos.first, pos.second, 30, 40, 1.0);
	player.addComponent<ColliderComponent>("player", pos.first, pos.second, 30, 40);
	player.addComponent<GravityComponent>();
	player.addComponent<MoveComponent, 0>();
	player.addComponent<ServerComponent>();
	player.addComponent<EquipmentComponent>();
	player.addComponent<HealthComponent>();
	player.addComponent<EventHandlerComponent, 0>(true);
	player.addGroup(groupLabels::groupPlayers);
	player.addGroup(groupLabels::groupColliders);
	ServerGame::getInstance().insertToEntityGroups(player.getID(), groupLabels::groupPlayers);
}

void forProjectile(Entity &projectile, std::pair<std::uint16_t, std::uint16_t> const &pos, bool faceRight) {
	spdlog::get("console")->info("ServerGenerator::forProjectile");
	Vector2D velocity = faceRight ? Vector2D(-10, 0) : Vector2D(10, 0);
	int projectileOffset = faceRight ? 40 : -50;
	projectile.addComponent<TransformComponent>(pos.first + projectileOffset, pos.second, 5, 5, 1.0);
	projectile.addComponent<ColliderComponent>("projectile", pos.first + projectileOffset, pos.second, 5, 5);
	projectile.addComponent<ProjectileComponent>(500, 10, velocity);
	projectile.addGroup(groupLabels::groupProjectiles);
	projectile.addGroup(groupLabels::groupColliders);
	ServerGame::getInstance().insertToEntityGroups(projectile.getID(), groupLabels::groupProjectiles);
}

void forWeapon(Entity &weapon, std::pair<std::uint16_t, std::uint16_t> const &pos) {
	weapon.addComponent<TransformComponent>(pos.first, pos.second, 20, 16, 1.0);
	weapon.addComponent<ColliderComponent>("weapon", pos.first, pos.second, 13, 18);
	weapon.addComponent<WearableComponent>();
	weapon.addComponent<GravityComponent>();
	weapon.addGroup(groupLabels::groupWeapons);
	weapon.addGroup(groupLabels::groupColliders);
	ServerGame::getInstance().insertToEntityGroups(weapon.getID(), groupLabels::groupWeapons);
}
} // namespace ServerGenerator

} // namespace FishEngine
