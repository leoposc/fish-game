#include "../include/fish_game/SocketManager.hpp"

SocketManager::SocketManager() : addrlen(sizeof(address)) {
  int opt = 1;

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Binding the socket to the network address and port
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Listening for incoming connections
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    close(server_fd);
    exit(EXIT_FAILURE);
  }
}

SocketManager::~SocketManager() { close(server_fd); }

void SocketManager::run() {
  int new_socket;
  char buffer[BUFFER_SIZE] = {0};

  // Accepting an incoming connection
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t *)&addrlen)) < 0) {
    perror("accept");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Reading data from the client
  int valread = read(new_socket, buffer, BUFFER_SIZE);
  std::cout << "Received: " << buffer << std::endl;

  // Echoing back the received message
  send(new_socket, buffer, valread, 0);
  std::cout << "Echo message sent" << std::endl;

  // Closing the socket
  close(new_socket);
}
