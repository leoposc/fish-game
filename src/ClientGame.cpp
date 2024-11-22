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

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

SDL_Renderer *ClientGame::renderer = nullptr;
SDL_Event ClientGame::game_event;
SDL_Rect ClientGame::camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

Manager clientManager;
AssetManager *ClientGame::assets = new AssetManager(&clientManager);

Map *clientMap;

auto &weapon(clientManager.addEntity());
auto &projectile(clientManager.addEntity());

ClientGame::ClientGame(const char *title, int xpos, int ypos, int width, int height, bool fullscreen) {
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
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		isRunning = false;
	}

	// load assets
	assets->addTexture("fish", "../../assets/RedFishSmall.png");
	assets->addTexture("pistol", "../../assets/PistolSmall.png");
	assets->addTexture("projectile", "../../assets/ProjectileSmall.png");
}

ClientGame::~ClientGame() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	// log clean
}

void ClientGame::init(int numPlayers) {

	// ================== init clientMap and assets ==================
	clientMap = new Map();
	std::cout << fs::path("../../maps") / mapPath << std::endl;
	clientMap->loadMap(fs::path("../../maps") / mapPath);

	// ================== init player ==================
	auto &player(clientManager.addEntity());
	auto initPos = clientMap->getInitialPos().at(0);
	ClientComponentsGenerator::forPlayer(player, initPos.first, initPos.second);
	players.push_back(&player);

	// ================== init enemies ==================
	for (int i = 1; i < numPlayers; ++i) {
		auto &opponent(clientManager.addEntity());
		auto initPos = clientMap->getInitialPos().at(i);
		ClientComponentsGenerator::forEnemy(opponent, initPos.first, initPos.second);
		players.push_back(&opponent);
	}
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
	clientManager.refresh();
	clientManager.update();
	Collision::checkWaterCollisions(&players, clientMap);
	Collision::checkPlattformCollisions(&players, clientMap);
}

// todo: does not work yet - prob pretty wrong
void ClientGame::zoomIn() {
	// Get the most outer players
	std::vector<Entity *> &players = clientManager.getGroup(groupLabels::groupPlayers);
	if (players.empty())
		return;

	int minX = players[0]->getComponent<TransformComponent>().position.getX();
	int minY = players[0]->getComponent<TransformComponent>().position.getY();
	int maxX = minX, maxY = minY;

	for (auto &player : players) {
		TransformComponent &transform = player->getComponent<TransformComponent>();
		int x = transform.position.getX();
		int y = transform.position.getY();
		minX = std::min(minX, x);
		minY = std::min(minY, y);
		maxX = std::max(maxX, x);
		maxY = std::max(maxY, y);
	}

	// int minWidth = std :

	minX = std::max(0, minX - 100);
	minY = std::max(0, minY - 100);
	maxX = std::min(SCREEN_WIDTH, maxX + 100);
	maxY = std::min(SCREEN_HEIGHT, maxY + 100);

	int width = (maxX - minX);
	int height = (maxY - minY);

	// create a minimum size for the camera
	width = std::max(width, SCREEN_WIDTH);
	height = std::max(height, SCREEN_HEIGHT);

	camera = {minX, minY, width, height};
	camera = {0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT};

#define DEBUG
#ifdef DEBUG
	std::cout << "Camera: " << camera.x << " " << camera.y << " " << camera.w << " " << camera.h << std::endl;
#endif
}

void ClientGame::render() {
	SDL_RenderClear(renderer);

	// zoomIn();
	// SDL_RenderSetViewport(renderer, &camera);

	clientMap->drawMap();

	clientManager.draw();

	// ===================== test if adaptive movement works ==========================
	// Entity *player = clientManager.getGroup(groupLabels::groupPlayers).back();
	// bool swimming = clientMap->isInWater(&player->getComponent<ColliderComponent>().collider);
	// player->getComponent<MoveComponent>().inWater = swimming;

	// bool collision = clientMap->checkPlattformCollisions(&player->getComponent<ColliderComponent>().collider);
	//  do something with the collision - don't know how to handle movements in x - y axis yet

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

	// SDL_RenderSetViewport(renderer, nullptr);

	SDL_RenderPresent(renderer);
}

bool ClientGame::joinGame(std::string ip) {

	// todo: connect to the server
	return false;
}

bool ClientGame::hasStarted() {
	mapPath = "map03.tmj";
	return true;

	// TODO: check if the game is started
	// fetch the number of players from the server and map
	// sth. like server->fetchState(struct with &mapPath, &numPlayers, &started);
	// return started;
}

bool ClientGame::running() {
	return isRunning;
}

void ClientGame::stop() {
	isRunning = false;
}

} // namespace FishEngine