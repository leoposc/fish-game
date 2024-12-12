
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
#include "../include/fish_game/TextureManager.hpp"
#include "../include/fish_game/Vector2D.hpp"

#include <SDL2/SDL.h>
#include <cereal/archives/binary.hpp>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>

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
		this->createPlayer("");
	}

	spdlog::get("console")->debug("ServerGame - init done");
}

void ServerGame::handleEvents() {
	if (MockServer::getInstance().pollEvent(ServerGame::game_event)) {
		if (game_event.type == SDL_QUIT) {
			isRunning = false;
		}
	}
}

void ServerGame::update() {
	serverManager.refresh();
	serverManager.update();
	// FishEngine::Collision::test();
	Collision::checkWaterCollisions(&players, serverMap);
	Collision::checkPlattformCollisions(&players, serverMap);
}

uint8_t ServerGame::createPlayer(const std::string &ip) {
	// update server state
	numPlayers++;

	auto &player(serverManager.addEntity());
	serverManager.addToGroup(&player, groupLabels::groupPlayers);
	ServerGenerator::forPlayer(player, serverMap->getPlayerSpawnpoints(numPlayers).at(numPlayers - 1));

	players.insert(std::make_pair(player.getID(), &player));
	playerIPs.insert(std::make_pair(player.getID(), ip));
	entityGroups.insert(std::make_pair(player.getID(), groupLabels::groupPlayers));

	return player.getID();
}

uint8_t ServerGame::acceptJoinRequest(const std::string &ip) {

	// Create player entity
	uint8_t id = createPlayer(ip);

	// send playerID to client
	// TODO

	return id;
}

void ServerGame::updatePlayerEvent() {
	// unpack the event from the frame
	SDL_Event event;
	uint8_t id;
	std::ifstream is("event.json");
	cereal::JSONInputArchive archive(is);
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

	std::ofstream os("gameState.bin", std::ios::binary);
	cereal::BinaryOutputArchive ar(os);

	// inform client about the number of the entities
	ar(players.size());

	for (auto &[id, group] : entityGroups) {
		// inform the client about the current entities
		ar(id, group);
		// serialize components of the entity
		spdlog::get("console")->debug("ServerGame - sendGameState: serialize entity with id: {}", id);
		ar(serverManager.getEntity(id));
	}

	// send the state to the client
	// TODO
	for (const auto &[id, ip] : playerIPs) {
		// send the state to the client
	}
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

bool ServerGame::checkCollisions(Entity *player) {
	return serverMap->checkPlattformCollisions(&player->getComponent<ColliderComponent>().collider);
}

} // namespace FishEngine