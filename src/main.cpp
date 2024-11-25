#include "../include/fish_game/ClientGame.hpp"
#include "../include/fish_game/GameInputEvents.hpp"
#include "../include/fish_game/NetworkClient.hpp"
#include "../include/fish_game/NetworkHost.hpp"
#include "../include/fish_game/ServerGame.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include <SDL2/SDL.h>
#include <arpa/inet.h>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>

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

FuncPtr combat() {
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	u_int32_t frameStart;
	int frameTime;

	while (client->running()) {
		frameStart = SDL_GetTicks();

		client->handleEvents();
		// server->handleEvents();

		// server->update();
		client->update();

		client->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	return mainMenu();
}

FuncPtr hostLobby() {
	std::cout << "Host Lobby - not implemented yet - back to mainMenu" << std::endl;

	return mainMenu();
}

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

	u_int32_t frameStart;
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
	server->init("map03.tmj", 2);

	std::cout << "====================GAME STARTED==================" << std::endl;
	return combat();
}

FuncPtr mainMenu() {
	std::cout << "Main Menu - choose" << std::endl;
	std::cout << "1. Host Lobby" << std::endl;
	std::cout << "2. Join Lobby" << std::endl;
	std::cout << "3. Quit" << std::endl;
	std::cout << "Enter choice: ";
	std::cin.clear();
	int choice;
	std::cin >> choice;
	switch (choice) {
	case 1:
		return hostLobby();
		break;
	case 2:
		return joinLobby();
		break;
	default:
		break;
	}
	return nullptr;
}

int main(int argc, char *argv[]) {

	client = new cG("Fish Game Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, false);
	server = new sG();

	mainMenu();

	return 0;
}

FuncPtr mainMenu() {
	std::cout << "Main Menu - choose" << std::endl;
	std::cout << "1. Host Lobby" << std::endl;
	std::cout << "2. Join Lobby" << std::endl;
	std::cout << "3. Quit" << std::endl;
	std::cout << "Enter choice: " << std::endl;
	std::cin.clear();
	int choice;
	std::cin >> choice;
	switch (choice) {
	case 1:
		return hostLobby();
		break;
	case 2:
		return joinLobby();
		break;
	default:
		break;
	}

	int main(int argc, char *argv[]) {
		spdlog::set_level(spdlog::level::debug);
		auto console = spdlog::stdout_color_mt("console");
		auto err_logger = spdlog::stderr_color_mt("stderr");

		// Create an existing SomeData object with a different largeVector
		std::vector<std::unique_ptr<MyRecord>> existingRecords;
		existingRecords.push_back(std::make_unique<MyRecord>(7, 8, 9.0f));
		SomeData existingData(std::move(existingRecords), "OldName", 24.24, {10, 20, 30});

		// Deserialize from the string into the existing object
		try {
			std::istringstream is(serializedData);
			cereal::JSONInputArchive archive(is);
			archive(existingData);
		} catch (const cereal::Exception &e) {
			std::cerr << "Deserialization error: " << e.what() << std::endl;
			return 1;
		}

		// Print the values of the loaded data
		std::cout << "Name: " << existingData.name << std::endl;
		std::cout << "Value: " << existingData.value << std::endl;
		for (const auto &record : existingData.data) {
			std::cout << "x: " << static_cast<int>(record->x) << ", y: " << static_cast<int>(record->y)
			          << ", z: " << record->z << std::endl;
		}

		// Print the largeVector to show it retains its previous value
		std::cout << "largeVector: ";
		for (const auto &val : existingData.largeVector) {
			std::cout << val << " ";
		}
		std::cout << std::endl;

		return 0;
	}
