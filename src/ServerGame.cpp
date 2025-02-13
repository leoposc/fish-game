
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

std::unique_ptr<ServerGame> ServerGame::instance = nullptr;

ServerGame::~ServerGame() {
	spdlog::get("console")->info("========== ServerGame deconstruction ==========");
	delete map;
	map = nullptr;
}
void ServerGame::printManager() {
	manager.print();
}

// @author: Leopold Schmid
void ServerGame::init(fs::path mapPath, bool combat) {

	// ================== init map and assets ==================
	isRunning = true;
	map = new Map();
	map->loadMap(fs::path("./maps") / mapPath);
	playerSpawnpoints = map->getPlayerSpawnpoints();

	// create existing players
	for (auto player : this->players) {
		this->createPlayer(player.getEntityId());
	}

	this->combat = combat;
	// spawn eventually weapons
	if (combat) {
		spawnWeapons();
	}

	spdlog::get("console")->info("ServerGame - Starting server loop");

	// start the server loop
	serverThread = std::thread(&ServerGame::serverLoop, this);
}

// @author: Leopold Schmid
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

	manager.refresh<ServerGame>();

	if (manager.getGroup(groupLabels::groupPlayers).empty()) {
		spdlog::get("console")->info("ServerGame - Game over");
	}

	// check if 30s passed
	if (SDL_GetTicks() - lastSpawnTime > 30000 && combat) {
		lastSpawnTime = SDL_GetTicks();
		spawnWeapons();
	}
}

// @author: Fabian Aster
void ServerGame::serverLoop() {
	const int updates_per_second = 60;
	const int loopDelay = 1000 / updates_per_second;
	uint32_t start;
	int loopTime;

	while (isRunning) {
		start = SDL_GetTicks();
		std::unique_lock<std::mutex> lock(serverMutex);
		serverCv.wait(lock, [this] { return isRunning; });

		handleJoinRequests();
		receivePlayerEvents();
		update();
		sendGameState();

		lock.unlock();
		loopTime = SDL_GetTicks() - start;
		if (loopDelay > loopTime) {
			SDL_Delay(loopDelay - loopTime);
		}
	}
}

// @author: Fabian Aster
uint8_t ServerGame::createPlayer() {
	auto &player(manager.addEntity());
	players.push_back(Player(player.getID()));

	// fetch a random spawnpoint and create player
	ServerGenerator::forPlayer(player, this->getPlayerSpawnpoint());

	return player.getID();
}

// @author: Fabian Aster
uint8_t ServerGame::createPlayer(int id) {
	auto &player(manager.addEntity(id));

	// fetch a random spawnpoint and create player
	ServerGenerator::forPlayer(player, this->getPlayerSpawnpoint());

	return player.getID();
}

// @author: Leopold Schmid
std::pair<std::uint16_t, std::uint16_t> ServerGame::getPlayerSpawnpoint() {
	int n_spawnpoints = playerSpawnpoints.size();

	// handle the lobby case, where all players spawn at the same point
	if (n_spawnpoints == 1) {
		return playerSpawnpoints.at(0);
	}

	std::pair<std::uint16_t, std::uint16_t> spawnpoint;
	int randomIndex = rand() % n_spawnpoints;
	spawnpoint = playerSpawnpoints.at(randomIndex);

	// erase the spawnpoint from the list
	playerSpawnpoints.erase(playerSpawnpoints.begin() + randomIndex);

	return spawnpoint;
}

// @author: Fabian Aster
uint8_t ServerGame::handleJoinRequests() {

	// send playerID to client
	auto clients = this->networkHost.getClients();
	if (old_clients.size() != clients.size()) {
		spdlog::get("console")->info("ServerGame - New client detected - list of all clients: ");
		for (const auto &client : clients) {
			spdlog::get("console")->info("client: {}", client);
		}

		// create new player
		createPlayer();

		old_clients = clients;
	}
	return 0;
}

// @author: Fabian Aster
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
		spdlog::get("network_logger")
		    ->debug("Parsed event: id: {}, event sym: {}, event type: {}", id, event.key.keysym.sym, event.type);

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

// @author: Leopold Schmid, Fabian Aster
void ServerGame::sendGameState() {

	manager.refresh<ServerGame>();

	// serialize the game state
	std::ostringstream os;
	cereal::BinaryOutputArchive ar(os);

	// inform client about the number of the entities
	ar(entityGroups.size());
	// spdlog::get("console")->debug("Sending game state with {} players", entityGroups.size());

	assert(entityGroups.size() == manager.getEntities().size());

	for (auto &[id, group] : entityGroups) {
		assert(id == manager.getEntity(id).getID());
		assert(id > 0);
		assert(group >= 0 && group < 5);

		// inform the client about the current entities
		ar(id, group);

		// serialize components of the entity
		ar(manager.getEntity(id));
	}

	auto serializedString = os.str();
	this->networkHost.updateState(serializedString);
}

// @author: Leopold Schmid
void ServerGame::spawnWeapons() {
	// ================== init weapons ==================
	auto spawnpoints = map->loadWeaponSpawnpoints();

	if (spawnpoints != nullptr) {

		// do no spawn weapons when existing weapon was not picked up
		auto &existingWeapons(manager.getGroup(groupLabels::groupWeapons));

		for (auto &spawnpoint : *spawnpoints) {
			spawnWeaponsAux(spawnpoint, existingWeapons);
		}
	}
}

// @author: Leopold Schmid
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

// @author: Leopold Schmid
void ServerGame::insertToEntityGroups(uint8_t id, groupLabels label) {
	entityGroups[id] = label;
}

// @author: Leopold Schmid
bool ServerGame::running() const {
	return isRunning;
}

// @author: Fabian Aster, Leopold Schmid
void ServerGame::stop() {
	spdlog::get("console")->info("=============== stopping ServerGame ===============");
	{
		std::lock_guard<std::mutex> lock(serverMutex);
		isRunning = false;
	}
	serverCv.notify_all();
	if (serverThread.joinable()) {
		serverThread.join();
	}

	manager.destroyEntities<ServerGame>();
	entityGroups.clear();
	delete map;
	map = nullptr;
}

// @author: Leopold Schmid
bool ServerGame::checkCollisions(Entity *e) {
	return map->checkCollisions(&e->getComponent<ColliderComponent>().collider);
}

void ServerGame::printEntityMetaData() {

	spdlog::get("console")->info("\n-------------------------------------------------");
	spdlog::get("console")->info("ServerGame - Entity Meta Data: ");
	spdlog::get("console")->info("Number of entities: {}", entityGroups.size());
	spdlog::get("console")->info("Number of entities in manager: {}", manager.getEntities().size());

	spdlog::get("console")->info("IDs inside entityGroups: ");
	for (auto &id : entityGroups) {
		spdlog::get("console")->info("ID: {}", id.first);
	}

	spdlog::get("console")->info("IDs inside manager: ");
	for (const auto &pair : manager.getEntities()) {
		spdlog::get("console")->info("ID: {}", pair.first);
	}
}

} // namespace FishEngine
