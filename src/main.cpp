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

  int sock = 0;
  struct sockaddr_in serv_addr;
  char buffer[BUFFER_SIZE] = {0};
  const char *hello = "Hello from client";

  // Creating socket file descriptor
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cerr << "Socket creation error" << std::endl;
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Converting IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    std::cerr << "Invalid address/ Address not supported" << std::endl;
    return -1;
  }

  // Connecting to the server
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cerr << "Connection Failed" << std::endl;
    return -1;
  }

  // Sending a message to the server
  send(sock, hello, strlen(hello), 0);
  std::cout << "Hello message sent" << std::endl;

  // Reading the server's response
  int valread = read(sock, buffer, BUFFER_SIZE);
  std::cout << "Received: " << buffer << std::endl;
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

  // Closing the socket
  close(sock);

  game->clean();

  return 0;
}
