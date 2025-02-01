
#include "../include/fish_game/ServerGame.hpp"
#include "../include/fish_game/AssetManager.hpp"
#include "../include/fish_game/Auxiliary.hpp"
#include "../include/fish_game/Collision.hpp"
#include "../include/fish_game/ECS/ColliderComponent.hpp"
#include "../include/fish_game/ECS/ComponentsGenerator.hpp"
#include "../include/fish_game/ECS/EventHandlerComponent.hpp"
#include "../include/fish_game/ECS/MoveComponent.hpp"
#include "../include/fish_game/ECS/ServerComponent.hpp"
#include "../include/fish_game/ECS/SpriteComponent.hpp"
#include "../include/fish_game/ECS/TransformComponent.hpp"
#include "../include/fish_game/ECS/WearableComponent.hpp"
#include "../include/fish_game/Map.hpp"
#include "../include/fish_game/MockServer.hpp"
#include "../include/fish_game/NetworkHost.hpp"
#include "../include/fish_game/Player.hpp"
#include "../include/fish_game/TextureManager.hpp"
#include "../include/fish_game/Vector2D.hpp"

#include <SDL2/SDL.h>
#include <cereal/archives/binary.hpp>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <vector>

namespace FishEngine {

ServerGame::ServerGame() : isRunning(false) {}

void ServerGame::printManager() {
	manager.print();
}

void ServerGame::init(fs::path mapPath) {
	// assert(manager.checkEmpty());
	// assert(map == nullptr);
	// assert(numPlayers > 0);
	// assert(players.empty());

	// ================== init map and assets ==================
	isRunning = true;
	map = new Map();
	map->loadMap(fs::path("../../maps") / mapPath);
	playerSpawnpoints = map->getPlayerSpawnpoints();

	// create existing players
	for (auto player : this->players) {
		this->createPlayer(player.getEntityId());
	}

	spdlog::get("console")->info("ServerGame - init done");
}

void ServerGame::handleEvents() {
	if (MockServer::getInstance().pollEvent()) {
		if (game_event.type == SDL_QUIT) {
			isRunning = false;
		}
	}
}

void ServerGame::update() {
	// delete dead entities
	manager.refresh<ServerGame>();

	// update the entities
	manager.update();

	// check collision
	Collision::isInWater(&manager.getGroup(groupLabels::groupPlayers), map);
	Collision::checkCollisions(&manager.getGroup(groupLabels::groupColliders), map);
	Collision::checkCollisions(&manager.getGroup(groupLabels::groupPlayers),
	                           &manager.getGroup(groupLabels::groupProjectiles));

	if (manager.getGroup(groupLabels::groupPlayers).empty()) {
		spdlog::get("console")->info("ServerGame - Game over");
		isRunning = false;
	}
}

uint8_t ServerGame::createPlayer() {
	auto &player(manager.addEntity());
	players.push_back(Player(player.getID()));

	// fetch a random spawnpoint and create player
	ServerGenerator::forPlayer(player, this->getPlayerSpawnpoint());

	return player.getID();
}

uint8_t ServerGame::createPlayer(int id) {
	auto &player(manager.addEntity(id));

	// fetch a random spawnpoint and create player
	ServerGenerator::forPlayer(player, this->getPlayerSpawnpoint());

	return player.getID();
}

std::pair<std::uint16_t, std::uint16_t> ServerGame::getPlayerSpawnpoint() {
	std::pair<std::uint16_t, std::uint16_t> spawnpoint;
	int n_spawnpoints = playerSpawnpoints.size();

	int randomIndex = rand() % n_spawnpoints;
	// todo: check why it is always the same spawnpoint
	spdlog::get("console")->info("Server Game - Random PlayerSpawnpoint index: {}", randomIndex);
	spawnpoint = playerSpawnpoints.at(randomIndex);

	// erase the spawnpoint from the list
	playerSpawnpoints.erase(playerSpawnpoints.begin() + randomIndex);

	return spawnpoint;
}

uint8_t ServerGame::handleJoinRequests() {
	static std::vector<std::string> old_clients;
	// send playerID to client
	auto clients = this->networkHost.getClients();
	if (old_clients.size() != clients.size()) {
		std::cout << "NEW CLIENT DETECTED" << std::endl;
		for (const auto &client : clients) {
			std::cout << client << std::endl;
		}

		// create new player
		createPlayer();

		old_clients = clients;
	}
	return 0;
}

void ServerGame::receivePlayerEvents() {
	// unpack the event from the frame
	SDL_Event event;
	uint8_t id;
	std::optional<std::string> action = this->networkHost.getAction();

	while (action.has_value()) {
		std::string unpackedAction = action.value();
		spdlog::get("network_logger")->debug("Action: {}", unpackedAction);

		std::istringstream is(unpackedAction);
		cereal::BinaryInputArchive archive(is);
		archive(id, event.key.keysym.sym, event.type);
		spdlog::get("console")->debug("Parsed event: id: {}, event sym: {}, event type: {}", id, event.key.keysym.sym,
		                              event.type);

		// TODO handle event
		// update the event inside the component of the player entity
		try {
			manager.print();
			ServerComponent &serCom = manager.getEntity(id).getComponent<ServerComponent>();
			serCom.setEvent(event);
		} catch (...) {
			spdlog::get("network_logger")->debug("Received event too late");
		}
		action = this->networkHost.getAction();
	}
}

void ServerGame::sendGameState() {

	manager.refresh<ServerGame>();

	// serialize the game state
	std::ostringstream os;
	cereal::BinaryOutputArchive ar(os);

	// inform client about the number of the entities
	ar(entityGroups.size());
	// spdlog::get("console")->debug("Sending game state with {} players", entityGroups.size());

	assert(entityGroups.size() == manager.getEntities().size());
	spdlog::get("stderr")->info("ServerGame - Number of entities: {}", entityGroups.size());
	for (auto &[id, group] : entityGroups) {
		assert(id == manager.getEntity(id).getID());
		assert(id > 0);
		assert(group >= 0 && group < 5);

		// inform the client about the current entities
		ar(id, group);

		// if projectile, send the direction
		if (group == groupLabels::groupProjectiles) {
			spdlog::get("console")->info("ServerGame - Sending projectile direction");
			ar(manager.getEntity(id).getComponent<TransformComponent>().faceRight);
		}

		// serialize components of the entity
		ar(manager.getEntity(id));
	}

	auto serializedString = os.str();
	this->networkHost.updateState(serializedString);
}

void ServerGame::spawnWeapons() {
	// ================== init weapons ==================
	auto spawnpoints = map->loadWeaponSpawnpoints();

	if (spawnpoints != nullptr) {

		// do no spawn weapons when existing weapon was not picked up
		auto &existingWeapons(manager.getGroup(groupLabels::groupWeapons));

		for (auto &spawnpoint : *spawnpoints) {
			spawnWeaponsAux(spawnpoint, existingWeapons);
			spdlog::get("console")->info("Weapon spawned at: {} {}", spawnpoint.first, spawnpoint.second);
		}
	}
}

void ServerGame::spawnWeaponsAux(const std::pair<std::uint16_t, std::uint16_t> &spawnpoint,
                                 const std::vector<Entity *> &existingWeapons) {

	// check for collision with existing weapons
	for (auto &weapon : existingWeapons) {

		auto &colliderA = weapon->getComponent<ColliderComponent>().collider;
		SDL_Rect colliderB = {spawnpoint.first, spawnpoint.second, 16, 16};

		// stop if there is a collision
		if (Collision::checkCollisions(colliderA, colliderB)) {
			return;
		}
	}

	// spawn the weapon
	auto &weapon(manager.addEntity());
	ServerGenerator::forWeapon(weapon, spawnpoint);
}

void ServerGame::insertToEntityGroups(uint8_t id, groupLabels label) {
	entityGroups[id] = label;
}

bool ServerGame::running() const {
	return isRunning;
}

void ServerGame::stop() {
	manager.destroyEntities<ServerGame>();
	entityGroups.clear();
	delete map;
	map = nullptr;
	isRunning = false;
}

bool ServerGame::checkCollisions(Entity *e) {
	return map->checkCollisions(&e->getComponent<ColliderComponent>().collider);
}

void ServerGame::printAllEntityIDs() {

	spdlog::get("stderr")->info("IDs inside entityGroups: ");
	for (auto &id : entityGroups) {
		spdlog::get("stderr")->info("ID: {}", id.first);
	}

	spdlog::get("stderr")->info("IDs inside manager: ");
	for (const auto &pair : manager.getEntities()) {
		spdlog::get("stderr")->info("ID: {}", pair.first);
	}
}

} // namespace FishEngine
