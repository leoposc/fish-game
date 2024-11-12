#include "../include/fish_game/ClientGame.hpp"
#include "../include/fish_game/AssetManager.hpp"
#include "../include/fish_game/Collision.hpp"
#include "../include/fish_game/ECS/ColliderComponent.hpp"
#include "../include/fish_game/ECS/ComponentsGenerator.hpp"
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

SDL_Renderer *ClientGame::renderer = nullptr;
SDL_Event ClientGame::game_event;
SDL_Rect ClientGame::camera = {0, 0, 800, 640};

Manager clientManager;
AssetManager *ClientGame::assets = new AssetManager(&clientManager);

Map *clientMap;

ClientGame::ClientGame() : cnt(0), isRunning(false) {}

ClientGame::~ClientGame() {}

void ClientGame::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen, int numPlayers) {
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		// log init
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			// log window creation
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			// log renderer created
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	} else {
		isRunning = false;
	}

	// ================== init clientMap and assets ==================
	clientMap = new Map();
	clientMap->loadMap("../../maps/map03.tmj");
	assets->addTexture("fish", "../../assets/RedFishSmall.png");

	// =================== init player===========================
	// scaling not working correctly, RedFish.png also very high resolution

	for (int i = 0; i < numPlayers; ++i) {
		auto &player(clientManager.addEntity());
		clientManager.addToGroup(&player, groupLabels::groupPlayers);
		auto initPos = clientMap->getInitialPos().at(i);
		ClientComponentsGenerator::forPlayer(player, initPos.first, initPos.second);
	}
}

void ClientGame::handleEvents() {
	// TODO: look for a better way to handle events since movement seems to be pretty slow
	SDL_PollEvent(&game_event);

	if (game_event.type == SDL_QUIT) {
		isRunning = false;
	}

	MockServer::getInstance().enqueueEvent(game_event);
}

void ClientGame::update() {
	// SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	// Vector2D playerPos = player.getComponent<MoveComponent>().position;

	clientManager.refresh();
	clientManager.update();

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

	// if (clientMap->isInWater(&player.getComponent<ColliderComponent>().collider)) {
	// 	player.getComponent<MoveComponent>().inWater = true;
	// } else {
	// 	player.getComponent<MoveComponent>().inWater = false;
	// }

	// ===================== outdated code ==========================
	// for (auto &c : clientManager.getGroup(groupColliders)) {

	//   if (Collision::AABB(c->getComponent<ColliderComponent>().collider,
	//                       playerCol)) {
	//     player.getComponent<MoveComponent>().position = playerPos;
	//   }
	// }

	// for (auto &p : clientManager.getGroup(groupProjectiles)) {
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

void ClientGame::render() {
	SDL_RenderClear(renderer);

	clientMap->drawMap();

	clientManager.draw();

	// ===================== test if adaptive movement works ==========================
	// Entity *player = clientManager.getGroup(groupLabels::groupPlayers).back();
	// bool swimming = clientMap->isInWater(&player->getComponent<ColliderComponent>().collider);
	// player->getComponent<MoveComponent>().inWater = swimming;

	// bool collision = clientMap->checkPlattformCollisions(&player->getComponent<ColliderComponent>().collider);
	//  do something with the collision - don't know how to handle movements in x - y axis yet

	// for (auto &t : clientManager.getGroup(groupLabels::groupMap)) {
	//   t->draw();
	// }

	// for (auto &t : clientManager.getGroup(groupLabels::groupColliders)) {
	//   t->draw();
	// }

	// for (auto &t : clientManager.getGroup(groupLabels::groupPlayers)) {
	//   t->draw();
	// }

	// for (auto &t : clientManager.getGroup(groupLabels::groupProjectiles)) {
	//   t->draw();
	// }

	SDL_RenderPresent(renderer);
}

void ClientGame::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	// log clean
}

bool ClientGame::running() {
	return isRunning;
}

void ClientGame::stop() {
	isRunning = false;
}

} // namespace FishEngine