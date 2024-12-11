#include "../include/fish_game/ClientGame.hpp"
#include "../include/fish_game/AssetManager.hpp"
#include "../include/fish_game/Auxiliary.hpp"
#include "../include/fish_game/Collision.hpp"
#include "../include/fish_game/ECS/ColliderComponent.hpp"
#include "../include/fish_game/ECS/ComponentsGenerator.hpp"
#include "../include/fish_game/ECS/MoveComponent.hpp"
#include "../include/fish_game/ECS/SpriteComponent.hpp"
#include "../include/fish_game/ECS/TransformComponent.hpp"
#include "../include/fish_game/ECS/WearableComponent.hpp"
#include "../include/fish_game/FontManager.hpp"
#include "../include/fish_game/Map.hpp"
#include "../include/fish_game/MockServer.hpp"
#include "../include/fish_game/TextureManager.hpp"
#include "../include/fish_game/Vector2D.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <spdlog/spdlog.h>

namespace FishEngine {

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

SDL_Renderer *ClientGame::renderer = nullptr;
SDL_Event ClientGame::game_event;
SDL_Rect ClientGame::camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

Manager clientManager;
AssetManager *ClientGame::assets = new AssetManager(&clientManager);

Map *clientMap;

// ================== helper functions ==================

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

	} else {
		spdlog::get("stderr")->error("SDL could not initialize! SDL_Error:{}", SDL_GetError());
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

// todo: sync player positions with server/ fetch them from server
// todo: sync weapon positions with server/ fetch them from server / spawn them periodically

void ClientGame::init(fs::path mp, int numPlayers, bool combat) {

	// ================== init game ==================
	isRunning = true;
	assert(clientManager.checkEmpty());
	// assert(numPlayers > 0);
	assert(players.empty());

	mapPath = mp;
	std::cout << "start: " << players.size() << std::endl;
	// ================== init clientMap and assets ==================
	clientMap = new Map();

	spdlog::get("console")->debug("{}/{}", fs::path("../../maps").string(), mapPath.string());

	clientMap->loadMap(fs::path("../../maps") / mapPath);

	// ================== init player ==================
	// auto &player(clientManager.addEntity());
	// auto initPos = clientMap->getPlayerSpawnpoints(numPlayers);
	// ClientGenerator::forPlayer(player, initPos.at(0));
	// players.push_back(&player);

	// if (combat) {
	// 	// ================== init enemies ==================
	// 	for (int i = 1; i < numPlayers; ++i) {
	// 		auto &opponent(clientManager.addEntity());
	// 		ClientGenerator::forEnemy(opponent, initPos.at(i));
	// 		players.push_back(&opponent);
	// 	}

	// 	// ================== init weapons ==================
	// 	spawnWeapons();
	// }
	spdlog::get("console")->debug(players.size());
	// serialization test
	{
		std::ofstream os("data2.json");
		cereal::JSONOutputArchive archive(os);
		archive(clientManager);
	}
}

void ClientGame::handleEvents() {
	SDL_PollEvent(&game_event);

	switch (game_event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
		// case F11 is pressed
	case SDL_KEYDOWN:
		if (game_event.key.keysym.sym == SDLK_F11) {
			spdlog::get("console")->debug(windowed);
			toggleWindowMode(window, &windowed);
		}
	default:
		break;
	}

	// MockServer::getInstance().enqueueEvent(game_event);
}

void ClientGame::update() {
	clientManager.refresh();
	clientManager.update();
	Collision::checkWaterCollisions(&players, clientMap);
	Collision::checkPlattformCollisions(&players, clientMap);
	clientMap->updateAnimations();
}

void ClientGame::render() {
	SDL_RenderClear(renderer);

	clientMap->drawMap();
	// clientManager.draw();

	for (auto &t : clientManager.getGroup(groupLabels::groupPlayers)) {
		t->draw();
	}

	for (auto &t : clientManager.getGroup(groupLabels::groupWeapons)) {
		t->draw();
	}

	for (auto &t : clientManager.getGroup(groupLabels::groupProjectiles)) {
		t->draw();
	}

	// assets->renderFonts();

	SDL_RenderPresent(renderer);
}

void ClientGame::spawnWeapons() {
	// ================== init weapons ==================
	auto spawnpoints = clientMap->loadWeaponSpawnpoints();
	for (auto &spawnpoint : *spawnpoints) {
		auto &weapon(clientManager.addEntity());
		ClientGenerator::forWeapon(weapon, spawnpoint);
	}
}

Manager *ClientGame::getManager() {
	return &clientManager;
}

std::string ClientGame::joinInterface() {

	clientMap = new Map();
	clientMap->loadMap(fs::path("../../maps/joinLobby.tmj"));

	FontManager gInputTextTexture(renderer, "../../assets/zd-bold.ttf");
	FontManager gPromptTextTexture(renderer, "../../assets/zd-bold.ttf", 26);

	SDL_Color textColor = {0, 0, 0, 255};
	SDL_Event event;
	SDL_StartTextInput();

	std::string inputText = "xxx.xxx.xxx.xxx";
	gInputTextTexture.loadFromRenderedText(inputText.c_str(), textColor);
	gPromptTextTexture.loadFromRenderedText("Enter IP Address", textColor);

	bool modified = false;
	bool running = true;
	bool renderText = true;

	while (running) {
		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				// return to main menu
				case SDLK_ESCAPE:
					inputText = "";
					running = false;
					break;
					// delete the last character
				case SDLK_BACKSPACE:
					if (inputText.length() > 0) {
						inputText.pop_back();
						renderText = true;
					}
					break;
				// handle comitting the input
				case SDLK_RETURN:
					// check if the input is a valid IPv4 address
					// if not, show an error message
					if (isValidIPv4(inputText)) {
						running = false;
					} else {
						modified = false;
						renderText = true;
						inputText = "Invalid IP Address";
						gInputTextTexture.loadFromRenderedText(inputText.c_str(), textColor);
					}
					break;
				// handle copy
				case SDLK_c:
					if (SDL_GetModState() & KMOD_CTRL) {
						SDL_SetClipboardText(inputText.c_str());
					}
					break;
				// handle paste
				case SDLK_v:
					if (SDL_GetModState() & KMOD_CTRL) {
						char *tmpText = SDL_GetClipboardText();
						inputText = tmpText;
						SDL_free(tmpText);

						renderText = true;
					}
					break;
				}
				break;
			case SDL_TEXTINPUT:
				if (modified) {
					inputText += event.text.text;
				} else {
					inputText = event.text.text;
					modified = true;
				}
				renderText = true;
				break;
			}
		}
		if (renderText) {
			if (inputText.length() == 0) {
				inputText = " ";
			}
			renderText = false;
			gInputTextTexture.loadFromRenderedText(inputText.c_str(), textColor);
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);

			clientMap->drawMap();
			gPromptTextTexture.render((SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, SCREEN_HEIGHT / 3 + 30);
			gInputTextTexture.render((SCREEN_WIDTH - gInputTextTexture.getWidth()) / 2,
			                         SCREEN_HEIGHT / 3 + 30 + gPromptTextTexture.getHeight());

			SDL_RenderPresent(renderer);
		}
	}

	SDL_StopTextInput();
	stop();
	return inputText;
}

void ClientGame::sendJoinRequest(std::string ip) {

	// send request and wait for response

	// playerID =
	// init the own player entity
	auto &ownPlayer(clientManager.addEntity());
	auto initPos = clientMap->getPlayerSpawnpoints(1);
	ClientGenerator::forPlayer(ownPlayer, initPos.at(0));
	players.push_back(&ownPlayer);
}

void ClientGame::receiveGameState() {
	Manager tmpManager;

	// deserialization
	std::ifstream is("gameState.json");
	cereal::JSONInputArchive archive(is);
	archive(clientManager);

	// check if new entities were added
	// for (auto &entity : tmpManager.getEntities()) {
	// 	if (!clientManager.entityExists(entity->getID())) {
	// 		clientManager.addEntity(entity->getID());

	// 		// check which group the entity belongs to
	// 		if (entity->hasGroup(groupLabels::groupPlayers)) {
	// 			ClientGenerator::forPlayer(*entity, clientMap->getPlayerSpawnpoints(1).at(0));
	// 			players.push_back(entity.get());
	// 		}
	// 		if (entity->hasGroup(groupLabels::groupProjectiles)) {
	// 			// ClientGenerator::forProjectile(*entity, clientMap->loadWeaponSpawnpoints()->at(0));
	// 		}
	// 		// if (entity->hasGroup(groupLabels::groupWeapons)) {
	// 		// 	ClientGenerator::forWeapon(*entity, clientMap->loadWeaponSpawnpoints()->at(0));
	// 		// }
	// 	}
	// }

	archive(clientManager);
}

void ClientGame::showIP(SDL_Texture *mTexture, int width, int height) {
	int x = 0;
	int y = 0;
	SDL_Rect renderQuad = {(SCREEN_WIDTH - width) / 2, SCREEN_HEIGHT / 3, width, height};

	SDL_RenderCopy(renderer, mTexture, NULL, &renderQuad);
}

Uint8 ClientGame::updateMainMenu() {

	if (Collision::checkExit(players.at(0), clientMap))
		return 0;

	if (Collision::checkStart(players.at(0), clientMap))
		return 1;

	return -1;
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
	clientManager.destroyEntities();
	players.clear();
	delete clientMap;
	clientMap = nullptr;
	isRunning = false;
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

	spdlog::get("console")->debug("Camera: {} {} {} {}", camera.x, camera.y, camera.w, camera.h);
}

} // namespace FishEngine
