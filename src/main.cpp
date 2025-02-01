#include "../include/fish_game/ClientGame.hpp"
#include "../include/fish_game/ECS/ComponentsGenerator.hpp"
#include "../include/fish_game/FontManager.hpp"
#include "../include/fish_game/GameInputEvents.hpp"
#include "../include/fish_game/MusicPlayer.hpp"
#include "../include/fish_game/NetworkClient.hpp"
#include "../include/fish_game/NetworkHost.hpp"
#include "../include/fish_game/ServerGame.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
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
FuncPtr hostLobby(bool isHost, bool needInit = true);
FuncPtr joinLobby();

// std::thread serverThread;
// std::mutex serverMutex;
// std::condition_variable serverCv;
bool serverRunning = false;

// void startServerThread() {
// 	serverRunning = true;
// 	serverThread = std::thread(serverLoop);
// }

FuncPtr combat(bool isHost) {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	const int FPS = 200;
	const int frameDelay = 1000 / FPS;

	MusicPlayer::getInstance().playCombatMusic();
	u_int32_t frameStart;
	int frameTime;

	if (isHost) {
		server->init("map04.tmj");
		server->spawnWeapons();
	}
	client->init("map04.tmj", true);

	while (client->running()) {
		frameStart = SDL_GetTicks();

		client->receiveGameState();
		client->handleEvents();
		client->update();
		client->render();

		spdlog::get("console")->debug("Client Manager:");
		client->getManager()->print();
		spdlog::get("console")->debug("my player id: {}", client->ownPlayerID);

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	client->stop();
	if (isHost) {
		server->stop();
	}

	spdlog::get("console")->info("Leaving combat...");

	return mainMenu();
	return hostLobby(isHost, false);
}

// todo: implement joinLobby
FuncPtr joinLobby() {
	std::string ip;
	ip = client->joinInterface();
	if (ip.empty()) {
		return mainMenu();
	}

	client->sendJoinRequest(ip, "join user");
	return hostLobby(false);
}

FuncPtr hostLobby(bool isHost, bool needInit) {
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	uint32_t frameStart;
	int frameTime;

	if (isHost) {
		server = &FishEngine::ServerGame::getInstance();
		server->init("hostLobby.tmj");
		client->networkClient.init("127.0.0.1", "host player");
	}

	client->init("hostLobby.tmj", false);
	client->createOwnPlayer();

	while (client->running()) {
		frameStart = SDL_GetTicks();

		client->receiveGameState();
		client->handleEvents();
		client->update();
		client->render();

		spdlog::get("network_logger")->debug("Client Manager:");
		client->getManager()->print();
		spdlog::get("network_logger")->debug("my player id: {}", client->ownPlayerID);

		switch (client->updateMainMenu()) {
		case 0:
			client->stop();
			if (isHost) {
				server->stop();
			}
			return mainMenu();
			break;
		case 3:
			client->stop();
			if (isHost) {
				server->stop();
			}
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
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	uint32_t frameStart;
	int frameTime;
	MusicPlayer::getInstance().playLobbyMusic();

	client->init("mainMenu.tmj", false);
	client->createOwnPlayer();

	while (client->running()) {
		frameStart = SDL_GetTicks();

		client->handleEvents();
		client->update();
		client->render();

		switch (client->updateMainMenu()) {
		case 0:
			client->stop();
			return nullptr;
			break;
		case 1:
			client->stop();
			return joinLobby();
			break;
		case 2:
			client->stop();
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
	auto network_logger = spdlog::stdout_color_mt("network_logger");
	auto socket_logger = spdlog::stdout_color_mt("socket_logger");
	auto pollEvent = spdlog::stdout_color_mt("pollEvent");
	socket_logger->set_level(spdlog::level::off);
	network_logger->set_level(spdlog::level::off);
	console->set_level(spdlog::level::off);
	err_logger->set_level(spdlog::level::debug);
	pollEvent->set_level(spdlog::level::off);

	auto &player = MusicPlayer::getInstance();

	player.setMusicVolume(MIX_MAX_VOLUME / 10);

	client = &FishEngine::ClientGame::getInstance();
	// joinLobby();
	// combat();
	mainMenu();

	// TODO: DESTROY other threads - 2 threads are still running
	spdlog::get("console")->info("Leaving Fish Game...");
	return 0;
}
