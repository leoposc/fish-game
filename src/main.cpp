#include "../include/fish_game/ClientGame.hpp"
#include "../include/fish_game/FontManager.hpp"
#include "../include/fish_game/GameInputEvents.hpp"
#include "../include/fish_game/NetworkClient.hpp"
#include "../include/fish_game/NetworkHost.hpp"
#include "../include/fish_game/ServerGame.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <arpa/inet.h>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#define PORT 8080
#define BUFFER_SIZE 1024

using cG = FishEngine::ClientGame;
using sG = FishEngine::ServerGame;

typedef void (*FuncPtr)();

cG *client = nullptr;
sG *server = nullptr;

FuncPtr mainMenu();
FuncPtr hostLobby(bool isHost);
FuncPtr joinLobby();

FuncPtr combat(bool isHost) {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "COMBAT STARTED" << std::endl;
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	u_int32_t frameStart;
	int frameTime;

	if (isHost) {
		server->init("map04.tmj", 0);
	}
	client->init("map04.tmj", 6, true);

	std::cout << "Combat init done!" << std::endl;

	while (client->running()) {
		frameStart = SDL_GetTicks();

		client->handleEvents();

		// problem: cliengame doesnt have the entity
		client->receiveGameState();
		client->handleEvents();
		client->update();
		client->render();

		spdlog::get("console")->debug("Client Manager:");
		client->getManager()->print();
		spdlog::get("console")->debug("my player id: {}", client->ownPlayerID);

		if (isHost) {
			server->handleJoinRequests();
			server->sendGameState();
			server->updatePlayerEvent();
			server->update();
		}

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	client->stop();
	server->stop();
	client->renderLoadingBar();

	spdlog::get("console")->info("Leaving combat...");

	return mainMenu();
}

// todo: implement joinLobby
FuncPtr joinLobby() {
	std::string ip;
	std::cout << "Enter IP address: ";
	ip = client->joinInterface();
	if (ip.empty()) {
		return mainMenu();
	}
	std::cout << "Joining lobby at " << ip << std::endl;

	client->sendJoinRequest(ip, "join user");

	client->renderLoadingBar();
	return hostLobby(false);
}

FuncPtr hostLobby(bool isHost) {
	std::cout << "Welcome to Fish Game HOST Lobby" << std::endl;

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	uint32_t frameStart;
	int frameTime;

	if (isHost) {
		server = &FishEngine::ServerGame::getInstance();
		server->init("hostLobby.tmj", 0);
		client->networkClient.init("127.0.0.1", "host player");
	}

	client->init("hostLobby.tmj", 6, false);

	client->createOwnPlayer();

	while (client->running()) {
		frameStart = SDL_GetTicks();

		client->receiveGameState();
		client->handleEvents();
		client->update();
		client->render();

		spdlog::get("console")->debug("Client Manager:");
		client->getManager()->print();
		spdlog::get("console")->debug("my player id: {}", client->ownPlayerID);

		if (isHost) {
			server->handleJoinRequests();
			server->sendGameState();
			server->updatePlayerEvent();
			server->update();
		}

		switch (client->updateMainMenu()) {

		case 0:
			std::cout << "Leaving main menu..." << std::endl;

			client->stop();
			if (isHost) {
				server->stop();
			}
			client->renderLoadingBar();
			return mainMenu();
			break;
		case 3:
			client->stop();
			if (isHost) {
				server->stop();
			}

			client->renderLoadingBar();
			return combat(isHost);
			break;

		default:
			break;
		}

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	client->stop();
	if (isHost) {
		server->stop();
	}
	return nullptr;
}

FuncPtr mainMenu() {

	std::cout << "Welcome to Fish Game" << std::endl;

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	uint32_t frameStart;
	int frameTime;

	client->init("mainMenu.tmj", 1, false);
	client->createOwnPlayer();

	while (client->running()) {
		frameStart = SDL_GetTicks();

		client->handleEvents();
		client->update();
		client->render();

		switch (client->updateMainMenu()) {
		case 0:
			client->stop();
			client->renderLoadingBar();
			return nullptr;
			break;
		case 1:
			client->stop();
			client->renderLoadingBar();
			return joinLobby();
			break;
		case 2:
			client->stop();
			client->renderLoadingBar();
			return hostLobby(true);
			break;
		default:
			break;
		}

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	return nullptr;
}

int main(int argc, char *argv[]) {
	spdlog::set_level(spdlog::level::debug);
	auto console = spdlog::stdout_color_mt("console");
	auto err_logger = spdlog::stderr_color_mt("stderr");

	client = &FishEngine::ClientGame::getInstance();

	client->renderLoadingBar();
	/* hostLobby(true); */
	// joinLobby();
	// combat();
	mainMenu();

	spdlog::get("console")->info("Leaving Fish Game...");

	return 0;
}
