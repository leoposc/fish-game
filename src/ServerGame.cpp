
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
AssetManager *ServerGame::assets = new AssetManager(&ServerManager);

Map *serverMap;

ServerGame::ServerGame() : cnt(0), isRunning(false) {}

ServerGame::~ServerGame() {}

void ServerGame::init(const char *title, int numPlayers) {
	// ================== init serverMap and assets ==================
	serverMap = new Map();
	serverMap->loadMap("../../maps/map03.tmj");
	assets->addTexture("fish", "../../assets/RedFishSmall.pname");

	// =================== init player===========================
	// scaling not working correctly, RedFish.png also very high resolution

	for (int i = 0; i < numPlayers; ++i) {
		auto &player(ServerManager.addEntity());
		ServerManager.addToGroup(&player, groupLabels::groupPlayers);
		auto initPos = serverMap->getInitialPos().at(i);
		ServerComponentsGenerator::forPlayer(player, initPos.first, initPos.second);
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
	// SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	// Vector2D playerPos = player.getComponent<MoveComponent>().position;

	ServerManager.refresh();
	ServerManager.update();

	// ================ GET CLIENT COMMANDS ==========================
	// => keyboard controller

	/*$
   | $$
/$$$$$$$  /$$$$$$         /$$$$$$$  /$$$$$$  /$$$$$$/$$$$   /$$$$$$
/$$__  $$ /$$__  $$       /$$_____/ /$$__  $$| $$_  $$_  $$ /$$__  $$
| $$  | $$| $$  \ $$      |  $$$$$$ | $$  \ $$| $$ \ $$ \ $$| $$$$$$$$
| $$  | $$| $$  | $$       \____  $$| $$  | $$| $$ | $$ | $$| $$_____/
|  $$$$$$$|  $$$$$$/       /$$$$$$$/|  $$$$$$/| $$ | $$ | $$|  $$$$$$$
\_______/ \______/       |_______/  \______/ |__/ |__/ |__/ \_______/

	            /$$                                       /$$       /$$
	           | $$                                      | $$      |__/
/$$$$$$$   /$$$$$$  /$$$$$$   /$$  /$$  /$$  /$$$$$$   /$$$$$$ | $$   /$$ /$$ /$$$$$$$   /$$$$$$
| $$__  $$ /$$__  $$|_  $$_/  | $$ | $$ | $$ /$$__  $$ /$$__  $$| $$  /$$/| $$| $$__  $$ /$$__  $$
| $$  \ $$| $$$$$$$$  | $$    | $$ | $$ | $$| $$  \ $$| $$  \__/| $$$$$$/ | $$| $$  \ $$| $$  \ $$
| $$  | $$| $$_____/  | $$ /$$| $$ | $$ | $$| $$  | $$| $$      | $$_  $$ | $$| $$  | $$| $$  | $$
| $$  | $$|  $$$$$$$  |  $$$$/|  $$$$$/$$$$/|  $$$$$$/| $$      | $$ \  $$| $$| $$  | $$|  $$$$$$$
|__/  |__/ \_______/   \___/   \_____/\___/  \______/ |__/      |__/  \__/|__/|__/  |__/ \____  $$
	                                                                                       /$$  \ $$
	                                                                                    . |  $$$$$$/
	                                                                                       \______/
.	         /$$                /$$$$$$   /$$$$$$
.  	      | $$                /$$__  $$ /$$__  $$
/$$$$$$$ /$$$$$$   /$$   /$$| $$  \__/| $$  \__/
/$$_____/|_  $$_/  | $$  | $$| $$$$    | $$$$
|  $$$$$$  | $$    | $$  | $$| $$_/    | $$_/
\____  $$  | $$ /$$| $$  | $$| $$      | $$
/$$$$$$$/  |  $$$$/|  $$$$$$/| $$      | $$
|_______/    \___/   \______/ |__/      |__*/

	// ================ UPDATE CLIENT POSITIONS AND SPRITES ==============

	// print collider data
	// std::cout << "Player collider: " << player.getComponent<ColliderComponent>().collider.x << " "
	//           << player.getComponent<ColliderComponent>().collider.y << " "
	//           << player.getComponent<ColliderComponent>().collider.w << " "
	//           << player.getComponent<ColliderComponent>().collider.h << std::endl;

	// if (serverMap->isInWater(&player.getComponent<ColliderComponent>().collider)) {
	// 	player.getComponent<MoveComponent>().inWater = true;
	// } else {
	// 	player.getComponent<MoveComponent>().inWater = false;
	// }

	// ===================== outdated code ==========================
	// for (auto &c : ServerManager.getGroup(groupColliders)) {

	//   if (Collision::AABB(c->getComponent<ColliderComponent>().collider,
	//                       playerCol)) {
	//     player.getComponent<MoveComponent>().position = playerPos;
	//   }
	// }

	// for (auto &p : ServerManager.getGroup(groupProjectiles)) {
	//   if (Collision::AABB(playerCol,
	//                       p->getComponent<ColliderComponent>().collider)) {
	//     std::cout << "Hit player" << std::endl;
	//     p->destroy();
	//   }
	// }

	/* TODO: move camera:
	  depends on all players positions
	camera.x = player.getComponent<MoveComponent>().position.x - 400;
	camera.y = player.getComponent<MoveComponent>().position.y - 320;

	if (camera.x < 0)
	  camera.x = 0;
	if (camera.y < 0)
	  camera.y = 0;
	if (camera.x > camera.w)
	  camera.x = camera.w;
	if (camera.y > camera.h)
	  camera.y = camera.h;
	*/
}

bool ServerGame::running() {
	return isRunning;
}

void ServerGame::stop() {
	isRunning = false;
}

} // namespace FishEngine