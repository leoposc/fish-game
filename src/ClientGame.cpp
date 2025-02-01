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
#include <cereal/archives/binary.hpp>
#include <fstream>
#include <future>
#include <iostream>
#include <spdlog/spdlog.h>

namespace FishEngine {

constexpr int SCREEN_WIDTH = 2048;
constexpr int SCREEN_HEIGHT = 1024;

// ================== helper functions ==================

void toggleWindowMode(SDL_Window *win, bool *windowed) {
	// Grab the mouse so that we don't end up with unexpected movement when the dimensions/position of the window
	// changes.
	SDL_SetRelativeMouseMode(SDL_TRUE);
	*windowed = !*windowed;
	if (*windowed) {
		int i = SDL_GetWindowDisplayIndex(win);
		// screenWidth = 1280;
		// screenHeight = 720;
		SDL_SetWindowFullscreen(win, 0);
	} else {
		int i = SDL_GetWindowDisplayIndex(win);
		SDL_Rect j;
		SDL_GetDisplayBounds(i, &j);
		// screenWidth = j.w;
		// screenHeight = j.h;
		SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	// recalculateResolution(); // This function sets appropriate font sizes/UI positions
}

ClientGame::ClientGame()
    : title("Fish Game Client"), xpos(SDL_WINDOWPOS_CENTERED), ypos(SDL_WINDOWPOS_CENTERED), width(SCREEN_WIDTH),
      height(SCREEN_HEIGHT), fullscreen(true) {

	int flags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		// log init
		window = SDL_CreateWindow(title, xpos, ypos, SCREEN_WIDTH, SCREEN_HEIGHT, flags);
		if (window) {
			// log window creation
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			// log renderer created
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	} else {
		spdlog::get("stderr")->error("SDL could not initialize! SDL_Error:{}", SDL_GetError());
		isRunning = false;
	}

	// load textures
	assets = new AssetManager(this->getManager());
	assets->addTexture("pistol", "../../assets/PistolSmall.png");
	assets->addTexture("projectile", "../../assets/ProjectileSmall.png");
	assets->addTexture("present", "../../assets/present.png");
}

ClientGame::~ClientGame() {
	/* networkClient.~NetworkClient(); */
	spdlog::get("console")->info("Deconstruciton of GameClient starts");
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	spdlog::get("console")->info("ClientGame deconstructed");
}

void ClientGame::init(fs::path mp, bool combat) {
	size_t progressUpdate = 0;
	auto future = std::async(std::launch::async, &FishEngine::ClientGame::startLoadingBar, this);

	isRunning = true;

	// ============ control if game was reset ===========
	assert(manager.checkEmpty());

	// ================ init clientMap ==================
	mapPath = mp;
	map = new Map();
	map->loadMap(fs::path("../../maps") / mapPath);

	// ================== init weapons ==================
	if (combat) {
		// spawnWeapons();
	}

	// reset entityGroups
	entityGroups.clear();

	// load assets
	fishSpriteID = 0;
	loadFishSprites();

	// render loading bar and wait for it to finish
	renderLoadingBar();
	future.wait();

	// remove all SDL_Events which occured during loading from the queue
	SDL_PumpEvents();
	SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
}

void ClientGame::loadFishSprites() {
	for (int i = 1; i <= numPlayers; i++) {
		std::string id = "fish0" + std::to_string(i);
		assets->addTexture(id, "../../assets/" + id + ".png");
	}
}

void ClientGame::handleEvents() {
	SDL_PollEvent(&game_event);
	// int eventCount = SDL_PeepEvents(nullptr, 0, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);

	switch (game_event.type) {
		// case quitting the game
	case SDL_QUIT:
		spdlog::get("console")->info("Leaving game");
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
}

void ClientGame::update() {
	// delete dead entities
	manager.refresh<ClientGame>();

	// // update the entities
	// spdlog::get("console")->debug("Updating entities");
	manager.update();

	// check for collisions
	Collision::isInWater(&manager.getGroup(groupLabels::groupPlayers), map);
	Collision::checkCollisions(&manager.getGroup(groupLabels::groupColliders), map);
	Collision::checkCollisions(&manager.getGroup(groupLabels::groupPlayers),
	                           &manager.getGroup(groupLabels::groupProjectiles));

	// // animate the map
	map->updateAnimations();

	// check if game is over TODO: just handle it in the server
	if (manager.getGroup(groupLabels::groupPlayers).empty()) {
		spdlog::get("console")->info("Game over - stopping game");
		isRunning = false;
	}
}

void ClientGame::render() const {
	SDL_RenderClear(renderer);

	map->drawMap();
	// manager.draw();

	for (auto &t : manager.getGroup_c(groupLabels::groupPlayers)) {
		t->draw();
	}

	for (auto &t : manager.getGroup_c(groupLabels::groupWeapons)) {
		t->draw();
	}

	for (auto &t : manager.getGroup_c(groupLabels::groupProjectiles)) {
		t->draw();
	}

	// assets->renderFonts();

	SDL_RenderPresent(renderer);
}

void ClientGame::createOwnPlayer() {
	ownPlayer = &manager.addEntity();
	auto initPos = map->getPlayerSpawnpoints().at(0);
	ClientGenerator::forPlayer(*ownPlayer, initPos, ++fishSpriteID);
	ownPlayerID = ownPlayer->getID();
}

void ClientGame::spawnWeaponsAux(const std::pair<std::uint16_t, std::uint16_t> &spawnpoint,
                                 const std::vector<Entity *> &existingWeapons) {

	// check for collision with existing weapons
	for (auto &weapon : existingWeapons) {

		auto &colliderA = weapon->getComponent<ColliderComponent>().collider;
		SDL_Rect colliderB = {spawnpoint.first, spawnpoint.second, 16, 16};

		// stop if there is a collision
		if (Collision::checkCollisions(colliderA, colliderB)) {
			return;
		}
	}

	// spawn the weapon
	auto &weapon(manager.addEntity());
	ClientGenerator::forWeapon(weapon, spawnpoint);
}

void ClientGame::spawnWeapons() {
	// ================== init weapons ==================
	auto spawnpoints = map->loadWeaponSpawnpoints();

	if (spawnpoints != nullptr) {

		// do no spawn weapons when existing weapon was not picked up
		auto &existingWeapons(manager.getGroup(groupLabels::groupWeapons));

		for (auto &spawnpoint : *spawnpoints) {
			spawnWeaponsAux(spawnpoint, existingWeapons);
		}
	}
}

Manager *ClientGame::getManager() {
	return &manager;
}

std::string ClientGame::joinInterface() {

	map = new Map();
	map->loadMap(fs::path("../../maps/joinLobby.tmj"));
	spdlog::get("console")->info("Map loaded");

	FontManager gInputTextTexture(renderer, "../../assets/zd-bold.ttf");
	FontManager gPromptTextTexture(renderer, "../../assets/zd-bold.ttf", 26);
	spdlog::get("console")->info("loaded textures");

	SDL_Color textColor = {0, 0, 0, 255};
	SDL_Event event;

	// remove all SDL_Events which occured during loading from the queue
	SDL_PumpEvents();
	SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
	SDL_StartTextInput();
	spdlog::get("console")->info("started input");

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

			map->drawMap();
			gPromptTextTexture.render((SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, SCREEN_HEIGHT / 3 + 55);
			gInputTextTexture.render((SCREEN_WIDTH - gInputTextTexture.getWidth()) / 2,
			                         SCREEN_HEIGHT / 3 + 65 + gPromptTextTexture.getHeight());

			SDL_RenderPresent(renderer);
		}
	}

	SDL_StopTextInput();
	stop();
	return inputText;
}

void ClientGame::sendJoinRequest(std::string ip, std::string username) {
	this->networkClient.init(ip, username);
}

void ClientGame::receiveGameState() {

	spdlog::get("console")->info("IDs inside entityGroups: ");
	for (auto &id : entityGroups) {
		spdlog::get("console")->info("ID: {}", id.first);
	}

	if (!this->networkClient.hasUpdate()) {
		// spdlog::get("console")->debug("skipped, no update received");
		return;
	}

	std::string serializedData = this->networkClient.getUpdate();
	std::istringstream is(serializedData);
	cereal::BinaryInputArchive ar(is);

	// fetch the number of the entities
	std::map<uint8_t, groupLabels> new_entityGroups;
	size_t numEntities;
	ar(numEntities);

	// first time while joining clear all entities - TODO: why not in init?
	if (!connected) {
		this->getManager()->destroyEntities<ClientGame>();
		spdlog::get("console")->debug("First join detected detruction");
	}

	spdlog::get("stderr")->info("Number of entities: {}", numEntities);
	// check if the entities are already loaded
	for (size_t i = 0; i < numEntities; ++i) {
		// read entity meta data from stream
		uint8_t _id;
		groupLabels group;
		ar(_id, group);
		const uint8_t id = _id;
		if (entityGroups.count(id)) {
			// case: entity already in manager
			new_entityGroups.insert(std::make_pair(id, group));
			// ...and remove from old list
			entityGroups.erase(id);
			// spdlog::get("console")->debug("Entity {} already in manager", id);
		} else {
			spdlog::get("console")->info("Entity {} of type not in manager", id);
			// spdlog::get("console")->info("Group: ")
			// create the entity
			auto &entity = manager.addEntity(id);

			// create the entity with the correct components
			switch (group) {
			case groupLabels::groupPlayers:
				// TODO: when joining combat its already connected -> no new eventhandler is created
				if (!connected && i == numEntities - 1) {
					this->ownPlayerID = id;
					ClientGenerator::forPlayer(entity, {0, 0}, ++fishSpriteID);
				} else if (connected && id == this->ownPlayerID) {
					ClientGenerator::forPlayer(entity, {0, 0}, ++fishSpriteID);
				} else {
					ClientGenerator::forEnemy(entity, {0, 0}, ++fishSpriteID);
				}
				break;
			case groupLabels::groupWeapons:
				ClientGenerator::forWeapon(entity, {0, 0});
				break;
			case groupLabels::groupProjectiles:
				spdlog::get("console")->info("Creating projectile");
				bool faceRight;
				ar(faceRight);
				ClientGenerator::forProjectile(entity, {0, 0}, faceRight);
				break;
			default:
				spdlog::get("stderr")->error("Unknown group type");
				throw std::runtime_error("Server is equesting to create an unknown group type.");
				break;
			}

			// add the entity to the manager
			new_entityGroups.insert(std::make_pair(id, group));
		}

		// update the values of the entity
		spdlog::get("console")->info("Calling getEntity inside receiveGameState!");
		spdlog::get("console")->info("ID: {}", id);
		spdlog::get("console")->info("Inside entityGroups: {}", entityGroups.count(id));
		ar(manager.getEntity(id));
		spdlog::get("console")->info("Returned from it!\n");
	}

	// destroy all entities which are not in the update
	for (auto &entity : entityGroups) {
		manager.getEntity(entity.first).destroy();
	}
	entityGroups = new_entityGroups;

	this->connected = true;
}

void ClientGame::showIP(SDL_Texture *mTexture, int width, int height) {
	int x = 0;
	int y = 0;
	SDL_Rect renderQuad = {(SCREEN_WIDTH - width) / 2, SCREEN_HEIGHT / 3, width, height};

	SDL_RenderCopy(renderer, mTexture, NULL, &renderQuad);
}

uint8_t ClientGame::updateMainMenu() const {

	if (Collision::checkBack(*ownPlayer, *map))
		return 0;

	if (Collision::checkJoin(*ownPlayer, *map))
		return 1;

	if (Collision::checkHost(*ownPlayer, *map))
		return 2;

	if (Collision::checkStart(*ownPlayer, *map))
		return 3;

	return -1;
}

bool ClientGame::running() const {
	return isRunning;
}

void ClientGame::stop() {
	manager.destroyEntities<ClientGame>();
	entityGroups.clear();
	delete map;
	map = nullptr;
	isRunning = false;
}

// todo: does not work yet - prob pretty wrong
// void ClientGame::zoomIn() {
// 	// Get the most outer players
// 	std::vector<Entity *> &players = manager.getGroup(groupLabels::groupPlayers);
// 	if (players.empty())
// 		return;

// 	int minX = players[0]->getComponent<TransformComponent>().position.getX();
// 	int minY = players[0]->getComponent<TransformComponent>().position.getY();
// 	int maxX = minX, maxY = minY;

// 	for (auto &player : players) {
// 		TransformComponent &transform = player->getComponent<TransformComponent>();
// 		int x = transform.position.getX();
// 		int y = transform.position.getY();
// 		minX = std::min(minX, x);
// 		minY = std::min(minY, y);
// 		maxX = std::max(maxX, x);
// 		maxY = std::max(maxY, y);
// 	}

// 	// int minWidth = std :

// 	minX = std::max(0, minX - 100);
// 	minY = std::max(0, minY - 100);
// 	maxX = std::min(SCREEN_WIDTH, maxX + 100);
// 	maxY = std::min(SCREEN_HEIGHT, maxY + 100);

// 	int width = (maxX - minX);
// 	int height = (maxY - minY);

// 	// create a minimum size for the camera
// 	width = std::max(width, SCREEN_WIDTH);
// 	height = std::max(height, SCREEN_HEIGHT);

// 	camera = {minX, minY, width, height};
// 	camera = {0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT};

// 	spdlog::get("console")->debug("Camera: {} {} {} {}", camera.x, camera.y, camera.w, camera.h);
// }

void ClientGame::startLoadingBar() {
	progressUpdate = 0;
	for (size_t i = 0; i < SCREEN_HEIGHT / 16; ++i) {
		++progressUpdate;
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

void ClientGame::renderLoadingBar() {
	Map *loadingScreen = new Map();
	loadingScreen->loadMap(fs::path("../../maps/loadingBar.tmj"), false);

	size_t progressBarsComplete = SCREEN_HEIGHT / 16;

	while (progressUpdate < progressBarsComplete) {
		loadingScreen->drawLoadingBar(progressUpdate);
		loadingScreen->updateAnimations();
		SDL_RenderPresent(ClientGame::renderer);
		SDL_Delay(1);
	}

	delete loadingScreen;
}

} // namespace FishEngine
