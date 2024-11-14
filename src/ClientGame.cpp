#include "../include/fish_game/ClientGame.hpp"
#include "../include/fish_game/AssetManager.hpp"
#include "../include/fish_game/Collision.hpp"
#include "../include/fish_game/ECS/ColliderComponent.hpp"
#include "../include/fish_game/ECS/ComponentsGenerator.hpp"
#include "../include/fish_game/ECS/MoveComponent.hpp"
#include "../include/fish_game/ECS/SpriteComponent.hpp"
#include "../include/fish_game/ECS/TransformComponent.hpp"
#include "../include/fish_game/ECS/WearableComponent.hpp"
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

auto &weapon(clientManager.addEntity());
auto &projectile(clientManager.addEntity());
ClientGame::ClientGame() : isRunning(false) {}

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
	assets->addTexture("pistol", "../../assets/PistolSmall.png");
	assets->addTexture("projectile", "../../assets/ProjectileSmall.png");

	for (int i = 0; i < numPlayers; ++i) {
		auto &player(clientManager.addEntity());
		clientManager.addToGroup(&player, groupLabels::groupPlayers);
		auto initPos = clientMap->getInitialPos().at(i);
		ClientComponentsGenerator::forPlayer(player, initPos.first, initPos.second);
	}

	// =================== init weapon ===========================
	// weapon.addComponent<ClientTransformComponent>(410, 250, 13, 18, 1.0);
	// weapon.addComponent<SpriteComponent>("pistol", false);
	// weapon.addComponent<ColliderComponent>("weapon", 410, 250, 13, 18);
	// weapon.addComponent<WearableComponent>();
	// weapon.addGroup(groupLabels::groupWeapons);

	// =================== init projectile =======================
	// projectile.addComponent<ClientTransformComponent>(0, 0, 16, 16, 1.0);
}

void toggleWindowMode(SDL_Window *win, bool *windowed) {
	// Grab the mouse so that we don't end up with unexpected movement when the dimensions/position of the window
	// changes.
	SDL_SetRelativeMouseMode(SDL_TRUE);
	std::cout << "first: " << *windowed << std::endl;
	*windowed = !*windowed;
	std::cout << "second: " << *windowed << std::endl;
	if (*windowed) {
		int i = SDL_GetWindowDisplayIndex(win);
		// screenWidth = 1280;
		// screenHeight = 720;
		SDL_SetWindowFullscreen(win, 0);
		std::cout << "Windowed" << std::endl;
	} else {
		int i = SDL_GetWindowDisplayIndex(win);
		SDL_Rect j;
		SDL_GetDisplayBounds(i, &j);
		// screenWidth = j.w;
		// screenHeight = j.h;
		SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
		std::cout << "Fullscreen" << std::endl;
	}
	// recalculateResolution(); // This function sets appropriate font sizes/UI positions
}

void ClientGame::handleEvents() {
	// TODO: look for a better way to handle events since movement seems to be pretty slow
	SDL_PollEvent(&game_event);

	switch (game_event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
		// case F11 is pressed
	case SDL_KEYDOWN:
		if (game_event.key.keysym.sym == SDLK_F11) {
			std::cout << windowed << std::endl;
			toggleWindowMode(window, &windowed);
		}
	default:
		break;
	}

	MockServer::getInstance().enqueueEvent(game_event);
}

void ClientGame::update() {
	// SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	// Vector2D playerPos = player.getComponent<MoveComponent>().position;

	clientManager.refresh();
	clientManager.update();
	std::cout << "finished updating!" << std::endl;

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

	for (auto &t : clientManager.getGroup(groupLabels::groupPlayers)) {
		t->draw();
	}

	for (auto &t : clientManager.getGroup(groupLabels::groupWeapons)) {
		t->draw();
	}

	for (auto &t : clientManager.getGroup(groupLabels::groupProjectiles)) {
		t->draw();
	}

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