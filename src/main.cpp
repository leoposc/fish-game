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

FuncPtr mainMenu();
FuncPtr hostLobby(bool isHost, bool needInit = true);
FuncPtr joinLobby();

FuncPtr combat(bool isHost) {
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	MusicPlayer::getInstance().playCombatMusic();
	u_int32_t frameStart;
	int frameTime;
	auto client = &FishEngine::ClientGame::getInstance();

	if (isHost) {
		auto server = &FishEngine::ServerGame::getInstance();
		server->init("map04.tmj", true);
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
		spdlog::get("console")->debug("my player id: {}", client->hostPlayerID);

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	client->stop();
	if (isHost) {
		auto server = &FishEngine::ServerGame::getInstance();
		server->stop();
	}

	spdlog::get("console")->info("Leaving combat...");

	FishEngine::ServerGame::resetInstance();
	client->reset();
	return mainMenu();
	return hostLobby(isHost, false);
}

FuncPtr joinLobby() {
	std::string ip;
	auto client = &FishEngine::ClientGame::getInstance();
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

	auto client = &FishEngine::ClientGame::getInstance();

	if (isHost) {
		auto server = &FishEngine::ServerGame::getInstance();
		server->init("hostLobby.tmj");
		client->networkClient.init("127.0.0.1", "host player");
	}

	client->init("hostLobby.tmj", false);

	while (client->running()) {
		frameStart = SDL_GetTicks();

		client->receiveGameState();
		client->handleEvents();
		client->update();
		client->render();

		spdlog::get("network_logger")->debug("Client Manager:");
		client->getManager()->print();
		spdlog::get("network_logger")->debug("my player id: {}", client->hostPlayerID);

		switch (client->updateMainMenu()) {
		case 0:
			client->stop();
			if (isHost) {
				FishEngine::ServerGame::getInstance().stop();
				FishEngine::ServerGame::resetInstance();
			}
			client->reset();
			return mainMenu();
			break;
		case 3:
			client->stop();
			if (isHost) {
				FishEngine::ServerGame::getInstance().stop();
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
		auto server = &FishEngine::ServerGame::getInstance();
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

	auto client = &FishEngine::ClientGame::getInstance();

	client->init("mainMenu.tmj", false);
	client->createHostPlayer();

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
	auto current_bug = spdlog::stdout_color_mt("cb");
	auto font_bug = spdlog::stdout_color_mt("fb");

	socket_logger->set_level(spdlog::level::off);
	network_logger->set_level(spdlog::level::info);
	console->set_level(spdlog::level::info);
	err_logger->set_level(spdlog::level::off);
	current_bug->set_level(spdlog::level::off);
	font_bug->set_level(spdlog::level::off);

	auto &player = MusicPlayer::getInstance();

	player.setMusicVolume(0);

	// joinLobby();
	// combat();
	mainMenu();

	// TODO: DESTROY other threads - 2 threads are still running
	spdlog::get("console")->info("Leaving Fish Game...");
	return 0;
}
