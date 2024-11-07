
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

#include <SDL2/SDL.h>
#include <iostream>

namespace FishEngine {

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::game_event;
SDL_Rect Game::camera = {0, 0, 800, 640};

Manager manager;
AssetManager *Game::assets = new AssetManager(&manager);

Map *map;

auto &player(manager.addEntity());

Game::Game() : cnt(0), isRunning(false) {}

Game::~Game() {}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen) {
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

	// etc implementation - temporary test code below
	map = new Map();
	map->loadMap("../../maps/map03.tmj");

	assets->addTexture("fish", "../../assets/RedFishSmall.png");

	// init player
	// scaling not working correctly, RedFish.png also very high resolution
	player.addComponent<MoveComponent>(400, 240, 45, 60, 1.0);
	player.addComponent<SpriteComponent>("fish", false);
	player.addComponent<KeyboardController>();
	// // // player.addComponent<ColliderComponent>("player");
	// // player.addComponent<TransformComponent>(2);
	// // player.addGroup(groupLabels::groupPlayers);
	player.addGroup(groupPlayers);
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

	manager.draw();

	map->drawMap();

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