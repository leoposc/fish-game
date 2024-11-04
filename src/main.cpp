#include "../include/fish_game/Game.hpp"
#include "../include/fish_game/NetworkHost.hpp"
#include "../include/fish_game/SocketManager.hpp"

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

int main(int argc, char *argv[]) {
  const int FPS = 60;
  const int frameDelay = 1000 / FPS;

  u_int32_t frameStart;
  int frameTime;

  game = new FishEngine::Game();
  game->init("Fish Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
             640, false);
  NetworkHost networkHost;
  SocketManager socketManager2(8080, false);
  socketManager2.sendMessage("S2 Hallo1\n");
  socketManager2.sendMessage("S2 Hallo2\n");

  // Reading the server's response
  while (game->running()) {
    std::cout << socketManager2.popMessage();
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
