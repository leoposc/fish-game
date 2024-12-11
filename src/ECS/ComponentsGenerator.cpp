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
	player.addGroup(ClientGame::groupLabels::groupPlayers);
}

void forEnemy(Entity &enemy, std::pair<std::uint16_t, std::uint16_t> const &pos) {
	enemy.addComponent<TransformComponent>(pos.first, pos.second, 45, 60, 1.0);
	enemy.addComponent<SpriteComponent>("fish", false);
	// enemy.addComponent<ClientComponent>();
	enemy.addComponent<ColliderComponent>("enemy", pos.first, pos.second, 45, 60);
	enemy.addComponent<MoveComponent>();
	enemy.addComponent<GravityComponent>();
	enemy.addGroup(ClientGame::groupLabels::groupEnemies);
	enemy.addGroup(ClientGame::groupLabels::groupPlayers);
}

void forWeapon(Entity &weapon, std::pair<std::uint16_t, std::uint16_t> const &pos) {
	weapon.addComponent<TransformComponent>(pos.first, pos.second, 13, 18, 1.0);
	weapon.addComponent<SpriteComponent>("pistol", false);
	weapon.addComponent<ColliderComponent>("weapon", pos.first, pos.second, 13, 18);
	weapon.addComponent<WearableComponent>();
	weapon.addGroup(ClientGame::groupLabels::groupWeapons);
}

void forProjectile(Entity &projectile, std::pair<std::uint16_t, std::uint16_t> const &pos) {}

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
	player.addGroup(ServerGame::groupLabels::groupPlayers);
}

void forProjectile(Entity &projectile, std::pair<std::uint16_t, std::uint16_t> const &pos) {}

} // namespace ServerGenerator

} // namespace FishEngine