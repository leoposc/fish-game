
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

SDL_Event ServerGame::game_event;

Manager serverManager;
AssetManager *ServerGame::assets = new AssetManager(&serverManager); // todo: need it?

ServerGame::ServerGame() : isRunning(false) {}

ServerGame::~ServerGame() {
	networkHost.~NetworkHost();
}

void ServerGame::printManager() {
	serverManager.print();
}

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

	// create existing players
	if (p_numPlayers == 0 && this->players.size() > 0) {
		for (auto player : this->players) {
			this->createPlayer(player.getEntityId());
		}
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
	// delete dead entities
	serverManager.refresh();

	// update the entities
	serverManager.update();

	// check collision
	Collision::isInWater(&serverManager.getGroup(ClientGame::groupLabels::groupPlayers), serverMap);
	Collision::checkCollisions(&serverManager.getGroup(ClientGame::groupLabels::groupPlayers), serverMap);
}

uint8_t ServerGame::createPlayer() {
	// update server state

	auto &player(serverManager.addEntity());
	serverManager.addToGroup(&player, groupLabels::groupPlayers);
	ServerGenerator::forPlayer(player, serverMap->getPlayerSpawnpoints(players.size() + 1).at(players.size()));

	players.push_back(Player(player.getID()));
	entityGroups.insert(std::make_pair(player.getID(), groupLabels::groupPlayers));

	return player.getID();
}

uint8_t ServerGame::createPlayer(int id) {
	// update server state

	auto &player(serverManager.addEntity(id));

	serverManager.addToGroup(&player, groupLabels::groupPlayers);
	ServerGenerator::forPlayer(player, serverMap->getPlayerSpawnpoints(players.size() + 1).at(players.size()));
	entityGroups.insert(std::make_pair(player.getID(), groupLabels::groupPlayers));

	return player.getID();
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
		spdlog::get("network_logger")->debug("No action received");
		return;
	}

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
		serverManager.print();
		ServerComponent &serCom = serverManager.getEntity(id).getComponent<ServerComponent>();
		serCom.setEvent(event);
	} catch (...) {
		spdlog::get("network_logger")->debug("Received event too late");
	}
}

void ServerGame::sendGameState() {
	if (SDL_GetTicks() < nextUpdate) {
		return;
	}

	nextUpdate = SDL_GetTicks() + updateRate_ms;

	std::ostringstream os;
	cereal::BinaryOutputArchive ar(os);

	// inform client about the number of the entities

	ar(players.size());

	for (auto &[id, group] : entityGroups) {

		// inform the client about the current entities
		ar(id, group, serverManager.getEntity(id).getComponent<TransformComponent>());

		// if projectile, send the direction
		if (group == groupLabels::groupProjectiles) {
			ar(serverManager.getEntity(id).getComponent<TransformComponent>().faceRight);
		}

		// serialize components of the entity
		ar(serverManager.getEntity(id));
	}

	auto serializedString = os.str();
	this->networkHost.updateState(serializedString);
}

bool ServerGame::running() {
	return isRunning;
}

void ServerGame::stop() {
	serverManager.destroyEntities();
	entityGroups.clear();
	delete serverMap;
	serverMap = nullptr;
	isRunning = false;
}

bool ServerGame::checkCollisions(Entity *e) {
	return serverMap->checkCollisions(&e->getComponent<ColliderComponent>().collider);
}

} // namespace FishEngine
