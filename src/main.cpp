#include "../include/fish_game/ClientGame.hpp"
#include "../include/fish_game/GameInputEvents.hpp"
#include "../include/fish_game/NetworkClient.hpp"
#include "../include/fish_game/NetworkHost.hpp"
#include "../include/fish_game/ServerGame.hpp"

#include <SDL2/SDL.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

FishEngine::ClientGame *clientGame = nullptr;
FishEngine::ServerGame *serverGame = nullptr;

int main(int argc, char *argv[]) {
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	u_int32_t frameStart;
	int frameTime;

	clientGame = new FishEngine::ClientGame();
	serverGame = new FishEngine::ServerGame();

	clientGame->init("Fish Game Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, false, 2);
	std::cout << "\nClient Game Initialized\n" << std::endl;
	serverGame->init("Fish Game Server", 2);

	while (clientGame->running()) {
		frameStart = SDL_GetTicks();

		clientGame->handleEvents();
		serverGame->handleEvents();

		serverGame->update();
		clientGame->update();

		clientGame->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	clientGame->clean();

	return 0;
}
