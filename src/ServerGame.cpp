
#include "../include/fish_game/ServerGame.hpp"
#include "../include/fish_game/AssetManager.hpp"
#include "../include/fish_game/Collision.hpp"
#include "../include/fish_game/ECS/ColliderComponent.hpp"
#include "../include/fish_game/ECS/ComponentsGenerator.hpp"
#include "../include/fish_game/ECS/EventHandlerComponent.hpp"
#include "../include/fish_game/ECS/MoveComponent.hpp"
#include "../include/fish_game/ECS/SpriteComponent.hpp"
#include "../include/fish_game/ECS/TransformComponent.hpp"
#include "../include/fish_game/Map.hpp"
#include "../include/fish_game/MockServer.hpp"
#include "../include/fish_game/TextureManager.hpp"
#include "../include/fish_game/Vector2D.hpp"

#include <SDL2/SDL.h>
#include <iostream>

namespace FishEngine {

SDL_Event ServerGame::game_event;

Manager ServerManager;
AssetManager *ServerGame::assets = new AssetManager(&ServerManager); // todo: need it?

Map *serverMap;

ServerGame::ServerGame() : isRunning(false) {}

ServerGame::~ServerGame() {}

void ServerGame::init(fs::path mapPath, int numPlayers) {
	// ================== init serverMap and assets ==================
	// mapPath = fs::path("../../maps") / mapPath;
	serverMap = new Map();
	serverMap->loadMap(fs::path("../../maps") / mapPath);
	// assets->addTexture("fish", "../../assets/RedFishSmall.pname"); any reason to load it here as well?

	// =================== init player===========================
	// scaling not working correctly, RedFish.png also very high resolution

	for (int i = 0; i < numPlayers; ++i) {
		auto &player(ServerManager.addEntity());
		ServerManager.addToGroup(&player, groupLabels::groupPlayers);
		auto initPos = serverMap->getInitialPos().at(i);
		ServerComponentsGenerator::forPlayer(player, initPos.first, initPos.second);
		players.push_back(&player);
	}
}

void ServerGame::handleEvents() {
	if (MockServer::getInstance().pollEvent(ServerGame::game_event)) {
		if (game_event.type == SDL_QUIT) {
			isRunning = false;
		}
	}
}

void ServerGame::update() {
	ServerManager.refresh();
	ServerManager.update();
	checkWaterCollisions();
	checkPlattformCollisions();
}

bool ServerGame::running() {
	return isRunning;
}

void ServerGame::stop() {
	isRunning = false;
}

void ServerGame::checkWaterCollisions() {
	for (auto &player : players) {
		player->getComponent<MoveComponent>().inWater =
		    serverMap->isInWater(&player->getComponent<ColliderComponent>().collider);
	}
}

void ServerGame::checkPlattformCollisions() {
	for (auto &player : players) {
		if (serverMap->checkPlattformCollisions(&player->getComponent<ColliderComponent>().collider)) {
			// check if player is moving downwards
			if (player->getComponent<TransformComponent>().velocity.getY() > 0) {
				player->getComponent<TransformComponent>().velocity.setY(0);
			}
		}
	}
}

bool ServerGame::checkCollisions(Entity *player) {
	return serverMap->checkPlattformCollisions(&player->getComponent<ColliderComponent>().collider);
}

} // namespace FishEngine