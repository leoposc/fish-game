
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

void ServerGame::init(fs::path mapPath, int numPlayers) {
	// assert(serverManager.checkEmpty());
	// assert(serverMap == nullptr);
	// assert(numPlayers > 0);
	// assert(players.empty());

	// ================== init serverMap and assets ==================
	isRunning = true;
	serverMap = new Map();
	serverMap->loadMap(fs::path("../../maps") / mapPath);

	// =================== init player===========================
	// scaling not working correctly, RedFish.png also very high resolution
	auto initPos = serverMap->getPlayerSpawnpoints(numPlayers);

	for (int i = 0; i < numPlayers; ++i) {
		auto &player(serverManager.addEntity());
		serverManager.addToGroup(&player, groupLabels::groupPlayers);
		ServerGenerator::forPlayer(player, initPos.at(i));
		players.push_back(&player);
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

void ServerGame::acceptJoinRequest(std::string ip) {
	// update server state
	// playerIDs.push_back(std::make_pair(ip, playerID));
	numPlayers++;

	// Create player entity
	auto &player(serverManager.addEntity());
	serverManager.addToGroup(&player, groupLabels::groupPlayers);
	ServerGenerator::forPlayer(player, serverMap->getPlayerSpawnpoints(1).at(0));
	players.push_back(&player);

	// send playerID to client
	// TODO
}

void ServerGame::updatePlayerEvent() {
	// unpack the event from the frame
	SDL_Event event;
	uint8_t id;
	std::ifstream is("event.json");
	cereal::JSONInputArchive archive(is);
	archive(id, event);

	// update the event inside the component of the player entity
	Entity *player = playerEntities[id];
	ServerComponent *serCom = &player->getComponent<ServerComponent>();
	serCom->setEvent(event);
}

void ServerGame::startGame() {

	// Choose a random map from 01 to 03
	std::string mapPath = "map0" + std::to_string(rand() % 3 + 1) + ".tmj";
	mapPath = "map03.tmj"; // todo:  only one map yet

	// initialize server
	init(mapPath, numPlayers);

	// send start signal to clients
	for (const auto &[ip, playerID] : playerIDs) {
		// send start signal
	}
}

void ServerGame::sendGameState() {

	// send game state
	std::ofstream os("gameState.json");
	cereal::JSONOutputArchive archive(os);
	archive(serverManager);
	for (const auto &[ip, playerID] : playerIDs) {

		// send game state to client
	}
}

bool ServerGame::running() {
	return isRunning;
}

void ServerGame::stop() {
	serverManager.destroyEntities();
	players.clear();
	delete serverMap;
	serverMap = nullptr;
	isRunning = false;
}

bool ServerGame::checkCollisions(Entity *player) {
	return serverMap->checkPlattformCollisions(&player->getComponent<ColliderComponent>().collider);
}

} // namespace FishEngine