#include "../include/fish_game/Game.hpp"
#include "../include/fish_game/GameInputEvents.hpp"
#include "../include/fish_game/NetworkClient.hpp"
#include "../include/fish_game/NetworkHost.hpp"

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

FishEngine::Game *game = nullptr;

int main(int argc, char *argv[])
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	u_int32_t frameStart;
	int frameTime;

	game = new FishEngine::Game();
	game->init("Fish Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);
	NetworkHost networkHost;

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	NetworkClient client("172.0.0.1", "test user");
	client.setEvent(InputEvent::Event::MOVE_DOWN);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	auto action = networkHost.getAction();
	if (action.has_value()) {
		std::cout << InputEvent::serialize(action.value());
	}

	// Reading the server's response
	while (game->running()) {
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->clean();

	return 0;
}
