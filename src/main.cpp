#include "../include/fish_game/ClientGame.hpp"
#include "../include/fish_game/GameInputEvents.hpp"
#include "../include/fish_game/NetworkClient.hpp"
#include "../include/fish_game/NetworkHost.hpp"
#include "../include/fish_game/ServerGame.hpp"

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

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	uint32_t frameStart;
	int frameTime;

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

	client = new cG("Fish Game Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, false);
	server = new sG();

	mainMenu();

	return 0;
}
