
#include "../include/fish_game/Game.hpp"
#include "../include/fish_game/AssetManager.hpp"
#include "../include/fish_game/Collision.hpp"
#include "../include/fish_game/ECS/ColliderComponent.hpp"
#include "../include/fish_game/ECS/KeyboardController.hpp"
#include "../include/fish_game/ECS/MoveComponent.hpp"
#include "../include/fish_game/ECS/SpriteComponent.hpp"
#include "../include/fish_game/ECS/TransformComponent.hpp"
#include "../include/fish_game/Map.hpp"
#include "../include/fish_game/TextureManager.hpp"
#include "../include/fish_game/Vector2D.hpp"
#include "../include/fish_game/ECS/ComponentsGenerator.hpp"


#include <SDL2/SDL.h>
#include <iostream>

namespace FishEngine {

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::game_event;
SDL_Rect Game::camera = {0, 0, 800, 640};

Manager manager;
AssetManager *Game::assets = new AssetManager(&manager);

Map *map;

Game::Game() : cnt(0), isRunning(false) {}

Game::~Game() {}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen, int numPlayers) {
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

	// ================== init map and assets ==================
	map = new Map();
	map->loadMap("../../maps/map03.tmj");
	assets->addTexture("fish", "../../assets/RedFishSmall.png");

	// =================== init player===========================
	// scaling not working correctly, RedFish.png also very high resolution

	for (int i = 0; i < numPlayers; ++i) {
		auto &player(manager.addEntity());
		manager.addToGroup(&player, groupLabels::groupPlayers);
		auto initPos = map->getInitialPos().at(i); 
		ComponentsGenerator::forPlayer(player, initPos.first, initPos.second);
	}
}

void Game::initCombat() {
	// init players
	// TODO: load every player from a file and change color
}

void Game::handleEvents() {
	// TODO: look for a better way to handle events since movement seems to be pretty slow
	SDL_PollEvent(&game_event);
	switch (game_event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
	// }
}

void Game::update() {
	// SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	// Vector2D playerPos = player.getComponent<MoveComponent>().position;

	manager.refresh();
	manager.update();

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

	// if (map->isInWater(&player.getComponent<ColliderComponent>().collider)) {
	// 	player.getComponent<MoveComponent>().inWater = true;
	// } else {
	// 	player.getComponent<MoveComponent>().inWater = false;
	// }

	// ===================== outdated code ==========================
	// for (auto &c : manager.getGroup(groupColliders)) {

	//   if (Collision::AABB(c->getComponent<ColliderComponent>().collider,
	//                       playerCol)) {
	//     player.getComponent<MoveComponent>().position = playerPos;
	//   }
	// }

	// for (auto &p : manager.getGroup(groupProjectiles)) {
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

void Game::render() {
	SDL_RenderClear(renderer);

	map->drawMap();

	manager.draw();

	// ===================== test if adaptive movement works ==========================
	Entity* player = manager.getGroup(groupLabels::groupPlayers).back();
	bool swimming = map->isInWater(&player->getComponent<ColliderComponent>().collider);
	player->getComponent<MoveComponent>().inWater = swimming;

	bool collision = map->checkPlattformCollisions(&player->getComponent<ColliderComponent>().collider);
	// do something with the collision - don't know how to handle movements in x - y axis yet

	// for (auto &t : manager.getGroup(groupLabels::groupMap)) {
	//   t->draw();
	// }

	// for (auto &t : manager.getGroup(groupLabels::groupColliders)) {
	//   t->draw();
	// }

	// for (auto &t : manager.getGroup(groupLabels::groupPlayers)) {
	//   t->draw();
	// }

	// for (auto &t : manager.getGroup(groupLabels::groupProjectiles)) {
	//   t->draw();
	// }

	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	// log clean
}

bool Game::running() {
	return isRunning;
}

void Game::stop() {
	isRunning = false;
}

} // namespace FishEngine