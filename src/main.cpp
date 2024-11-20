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

cG *clientGame = nullptr;
sG *serverGame = nullptr;

void combat(cG *clientGame, sG *serverGame) {
	clientGame->handleEvents();
	serverGame->handleEvents();

	serverGame->update();
	clientGame->update();

	clientGame->render();
}

void loop(const int FPS, cG *client, sG *server, std::function<void(cG *, sG *)> func) {
	// todo: read FPS from config file
	const int frameDelay = 1000 / FPS;

	u_int32_t frameStart;
	int frameTime;

	while (clientGame->running()) {
		frameStart = SDL_GetTicks();

		func(client, server);

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
}

int main(int argc, char *argv[]) {
	const int FPS = 60;

	clientGame = new cG();
	serverGame = new sG();

	clientGame->init("Fish Game Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, false, 2);
	serverGame->init("Fish Game Server", 2);

	loop(FPS, clientGame, serverGame, combat);

	clientGame->clean();

	return 0;
}
