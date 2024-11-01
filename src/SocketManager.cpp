#include "../include/fish_game/SocketManager.hpp"
#include <cstdlib>

SocketManager::SocketManager(int port, bool host) : addrlen(sizeof(address)) {
  if (host) {
    setupServer(port);
  } else {
    setupClient(port);
  }

  this->workerThread = std::thread(&SocketManager::run, this);
  std::cout << "Thread created \n";
}

SocketManager::~SocketManager() {
  std::cout << "SocketManager deconstructed \n";
  {
    std::lock_guard<std::mutex> lock(mtx);
    this->stopThread = true;
  }
  this->cv.notify_all();
  if (workerThread.joinable()) {
    workerThread.join();
  }
  close(this->socket_id);
  if (this->server_fd > 0) {
    close(this->server_fd);
  }
  std::cout << "SocketManager deconstructed finished\n";
}

void SocketManager::setupServer(int port) {
  int opt = 1;

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

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

  if ((this->socket_id = accept(server_fd, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen)) < 0) {
    perror("accept");
    close(server_fd);
    exit(EXIT_FAILURE);
  }
}

void SocketManager::setupClient(int port) {
  // Creating socket file descriptor
  if ((socket_id = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_port = htons(port);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0) {
    perror("Invalid address/ Address not supported");
    exit(EXIT_FAILURE);
  }

  // Connect to the server
  if (connect(socket_id, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Connection Failed");
    exit(EXIT_FAILURE);
  }
}

void SocketManager::run() {
  while (true) {
    char buffer[BUFFER_SIZE] = {0};
    int valread = read(this->socket_id, buffer, BUFFER_SIZE);

    std::lock_guard<std::mutex> lock(mtx);
    if (valread < 0) {
      std::cout << "Problem while reading \n";
      exit(EXIT_FAILURE);
    } else if (valread == 0) {
      std::cout << "Client disconnected \n";
      exit(EXIT_FAILURE);
    }
    this->messages.push_back(buffer);
    std::cout << "Received: " << buffer << std::endl;

    if (stopThread) {
      return;
    }
  }
}

std::string SocketManager::popMessage() {
  std::lock_guard<std::mutex> lock(mtx);
  if (messages.empty()) {
    return ""; // Return an empty string or handle the case as needed
  }
  auto message = messages.back();
  this->messages.pop_back();
  return message;
}

void SocketManager::sendMessage(std::string message) {
  char buffer[BUFFER_SIZE] = {0};
  strncpy(buffer, message.c_str(), BUFFER_SIZE - 1); // Copy message to buffer

  // Send the message to the socket
  ssize_t bytes_sent = send(this->socket_id, buffer, strlen(buffer), 0);
  if (bytes_sent < 0) {
    perror("send");
  } else {
    std::cout << "Echo message sent: " << message << std::endl;
  }
}
