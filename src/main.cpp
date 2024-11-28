#include "../include/fish_game/ClientGame.hpp"
#include "../include/fish_game/ECS/ECS.hpp"
#include "../include/fish_game/GameInputEvents.hpp"
#include "../include/fish_game/NetworkClient.hpp"
#include "../include/fish_game/NetworkHost.hpp"
#include "../include/fish_game/ServerGame.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include <SDL2/SDL.h>
#include <arpa/inet.h>
#include <cstring>
#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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

FuncPtr combat() {
	std::cout << "COMBAT STARTED" << std::endl;
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	u_int32_t frameStart;
	int frameTime;

	client->init("map03.tmj", 2, true);
	server->init("map03.tmj", 2);

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

	return mainMenu();
}

// todo: implement joinLobby
FuncPtr joinLobby() {
	return hostLobby();
}

FuncPtr hostLobby() {

	std::cout << "Welcome to Fish Game" << std::endl;


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

FuncPtr joinLobby() {
	std::cout << "Join Lobby" << std::endl;
	std::cout << "Enter IP: " << std::endl;
	std::string ip;
	std::cin >> ip;
	client->joinGame(ip);
	// ================================================================
	// being able to swim a little around with the other players
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	uint32_t frameStart;
	int frameTime;
	// client->init("map03.tmj", 2);
	// server->init("map03.tmj", 2);
	while (!client->hasStarted()) {
		// 	frameStart = SDL_GetTicks();
		// 	client->handleEvents();
		// 	server->handleEvents();
		// 	server->update();
		// 	client->update();
		// 	client->render();
		// 	frameTime = SDL_GetTicks() - frameStart;
		// 	if (frameDelay > frameTime) {
		// 		SDL_Delay(frameDelay - frameTime);
		// 	}
	}
	// ======================== INIT GAME ============================
	client->init(2);

	// serialization test
	auto manager = client->getManager();
	MyRecord record = MyRecord();

	std::ostringstream os;
	{
		cereal::JSONOutputArchive archive(os);
		archive(record);
	}

	std::string serializedData = os.str();
	std::cout << "Serialized Data: " << serializedData << std::endl;

	server->init("map03.tmj", 2);

	client->init("lobby.tmj", 1, false);
	server->init("lobby.tmj", 1);

	std::cout << "init done!" << std::endl;

	while (client->running()) {
		frameStart = SDL_GetTicks();

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

	client->init("mainMenu.tmj", 1, false);
	server->init("mainMenu.tmj", 1);

	while (client->running()) {
		frameStart = SDL_GetTicks();

		client->handleEvents();
		client->update();
		client->render();

		switch (client->updateMainMenu()) {
		case 0:
			std::cout << "Leaving main menu..." << std::endl;
			client->stop();
			server->stop();
			return nullptr;
			break;
		case 1:
			client->stop();
			server->stop();
			return hostLobby();
			break;
		case 2:
			client->stop();
			server->stop();
			return joinLobby();
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

	client = new cG("Fish Game Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, false);
	server = new sG();

	combat();
	// mainMenu();

	return 0;
}
