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
FuncPtr hostLobby();
FuncPtr joinLobby();

class MyRecord {
  public:
	uint8_t x, y;
	float z;

	MyRecord() = default;
	MyRecord(uint8_t x, uint8_t y, float z) : x(x), y(y), z(z) {}

	template <class Archive>
	void serialize(Archive &ar) {
		ar(x, y, z);
	}
};

FuncPtr combat() {
	std::cout << "COMBAT STARTED" << std::endl;
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	u_int32_t frameStart;
	int frameTime;

	client->init("map03.tmj", 4, true);
	// server->init("map03.tmj", 4);

	std::cout << "Combat init done!" << std::endl;

	while (client->running()) {
		frameStart = SDL_GetTicks();

		client->handleEvents();
		client->update();
		client->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	client->stop();
	server->stop();

	return mainMenu();
}

// todo: implement joinLobby
FuncPtr joinLobby() {
	std::string ip = client->joinInterface();
	if (ip.empty()) {
		return mainMenu();
	}
	std::cout << "Joining lobby at " << ip << std::endl;
	client->sendJoinRequest(ip);

	// todo: connect to host

	return hostLobby();
}

FuncPtr hostLobby() {

	std::cout << "Welcome to Fish Game" << std::endl;

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	uint32_t frameStart;
	int frameTime;

	std::string ownIP = "127.0.0.1";

	client->init("lobby.tmj", 1, false);
	server->init("lobby.tmj", 1);
	FishEngine::ClientGame::assets->loadFromRenderedText(ownIP, "../../assets/zd-bold.ttf", 24, {0, 0, 0, 255});

	// todo: start server - open socket

	while (client->running()) {
		frameStart = SDL_GetTicks();

		// todo: wait for players logic and sync with clients

		client->handleEvents();
		client->update();
		client->render();

		switch (client->updateMainMenu()) {
		case 0:
			std::cout << "Leaving main menu..." << std::endl;

			client->stop();
			server->stop();
			return mainMenu();
			break;
		case 1:
			client->stop();
			server->stop();
			return combat();
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

FuncPtr mainMenu() {

	std::cout << "Welcome to Fish Game" << std::endl;

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	uint32_t frameStart;
	int frameTime;

	client->init("mainMenu.tmj", 0, false);
	server->init("mainMenu.tmj", 0);

	client->sendJoinRequest("ip");
	server->acceptJoinRequest("ip");

	server->sendGameState();
	client->receiveGameState();

	// while (client->running()) {
	// 	frameStart = SDL_GetTicks();

	// 	client->handleEvents();
	// 	client->update();
	// 	client->render();

	// 	switch (client->updateMainMenu()) {
	// 	case 0:
	// 		std::cout << "Leaving main menu..." << std::endl;
	// 		client->stop();
	// 		server->stop();
	// 		return nullptr;
	// 		break;
	// 	case 1:
	// 		client->stop();
	// 		server->stop();
	// 		return hostLobby();
	// 		break;
	// 	case 2:
	// 		client->stop();
	// 		server->stop();
	// 		return joinLobby();
	// 		break;
	// 	default:
	// 		break;
	// 	}

	// 	frameTime = SDL_GetTicks() - frameStart;
	// 	if (frameDelay > frameTime) {
	// 		SDL_Delay(frameDelay - frameTime);
	// 	}
	// }
	return nullptr;
}

int main(int argc, char *argv[]) {
	spdlog::set_level(spdlog::level::debug);
	auto console = spdlog::stdout_color_mt("console");
	auto err_logger = spdlog::stderr_color_mt("stderr");

	client = new cG("Fish Game Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, false);
	server = new sG();

	// joinLobby();
	// combat();
	mainMenu();

	return 0;
}