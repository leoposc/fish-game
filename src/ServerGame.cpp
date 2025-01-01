
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
#include "../include/fish_game/TextureManager.hpp"
#include "../include/fish_game/Vector2D.hpp"

#include <SDL2/SDL.h>
#include <cereal/archives/binary.hpp>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <vector>

namespace FishEngine {

SDL_Event ServerGame::game_event;

Manager serverManager;
AssetManager *ServerGame::assets = new AssetManager(&serverManager); // todo: need it?

Map *serverMap;

ServerGame::ServerGame() : isRunning(false) {}

ServerGame::~ServerGame() {}

void ServerGame::init(fs::path mapPath, int p_numPlayers) {
	// assert(serverManager.checkEmpty());
	// assert(serverMap == nullptr);
	// assert(numPlayers > 0);
	// assert(players.empty());

	// ================== init serverMap and assets ==================
	isRunning = true;
	serverMap = new Map();
	serverMap->loadMap(fs::path("../../maps") / mapPath);

	// =================== init player===========================
	for (size_t i = 0; i < p_numPlayers; i++) {
		this->createPlayer();
	}

	spdlog::get("console")->debug("ServerGame - init done");
}

void ServerGame::handleEvents() {
	if (MockServer::getInstance().pollEvent()) {
		if (game_event.type == SDL_QUIT) {
			isRunning = false;
		}
	}
}

void ServerGame::update() {
	serverManager.refresh();
	serverManager.update();
	Collision::isInWater(&serverManager.getGroup(ClientGame::groupLabels::groupPlayers), serverMap);
	Collision::checkCollisions(&serverManager.getGroup(ClientGame::groupLabels::groupPlayers), serverMap);
}

uint8_t ServerGame::createPlayer() {
	// update server state
	numPlayers++;

	auto &player(serverManager.addEntity());
	serverManager.addToGroup(&player, groupLabels::groupPlayers);
	ServerGenerator::forPlayer(player, serverMap->getPlayerSpawnpoints(numPlayers).at(numPlayers - 1));

	players.insert(std::make_pair(player.getID(), &player));
	entityGroups.insert(std::make_pair(player.getID(), groupLabels::groupPlayers));

	return player.getID();
}

// NOT really needed, is inside of network host
uint8_t ServerGame::handleJoinRequests() {

	static std::vector<std::string> old_clients;
	// send playerID to client
	// TODO: poll if new player joined, if a player joined create playerid here and create mapping between socket_id and
	// this id
	auto clients = this->networkHost.getClients();
	if (old_clients.size() != clients.size()) {
		std::cout << "NEW CLIENT DETECTED" << std::endl;
		for (const auto &client : clients) {
			std::cout << client << std::endl;
		}

		// TODO: create new player
		createPlayer();

		old_clients = clients;
	}

	return 0;
}

void ServerGame::updatePlayerEvent() {
	// unpack the event from the frame
	SDL_Event event;
	uint8_t id;
	std::optional<std::string> action = this->networkHost.getAction();
	if (!action.has_value()) {
		std::cout << "No action available" << std::endl;
		return;
	}

	std::string unpackedAction = action.value();
	std::cout << "Action: " << unpackedAction << std::endl;

	std::istringstream is;
	cereal::BinaryInputArchive archive(is);
	archive(id, event);

	// update the event inside the component of the player entity
	ServerComponent *serCom = &players[id]->getComponent<ServerComponent>();
	serCom->setEvent(event);
}

void ServerGame::startGame() {

	// Choose a random map from 01 to 03
	std::string mapPath = "map0" + std::to_string(rand() % 3 + 1) + ".tmj";
	mapPath = "map03.tmj"; // todo:  only one map yet

	// initialize server
	init(mapPath, numPlayers);

	// send start signal to clients
	for (const auto &[playerID, ip] : playerIPs) {
		// send start signal
	}
}

void ServerGame::sendGameState() {

	std::ostringstream os;
	cereal::BinaryOutputArchive ar(os);

	// inform client about the number of the entities
	ar(players.size());
	std::cout << "size: " << static_cast<int>(players.size()) << std::endl;

	for (auto &[id, group] : entityGroups) {
		spdlog::get("console")->debug("ServerGame - sendGameState: serialize entity with id: {}", id);

		// inform the client about the current entities
		ar(id, group);
		std::cout << "ID: " << static_cast<int>(id) << std::endl;

		// if projectile, send the direction
		if (group == groupLabels::groupProjectiles) {
			ar(serverManager.getEntity(id).getComponent<TransformComponent>().faceRight);
		}

		// serialize components of the entity
		ar(serverManager.getEntity(id));
	}

	auto serializedString = os.str();
	spdlog::get("console")->debug("SeverGame: Serialized string: " + serializedString);
	this->networkHost.updateState(serializedString);
}

bool ServerGame::running() {
	return isRunning;
}

void ServerGame::stop() {
	serverManager.destroyEntities();
	players.clear();
	entityGroups.clear();
	delete serverMap;
	serverMap = nullptr;
	isRunning = false;
}

bool ServerGame::checkCollisions(Entity *e) {
	return serverMap->checkCollisions(&e->getComponent<ColliderComponent>().collider);
}

} // namespace FishEngine
