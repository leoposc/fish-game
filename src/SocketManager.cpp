#include "../include/fish_game/SocketManager.hpp"

#include "spdlog/spdlog.h"

SocketManager::SocketManager() {
	spdlog::get("socket_logger")->debug("SocketManager initialized");
}

void SocketManager::init(int port, std::string ip, bool host) {
	this->host = host;
	if (host) {
		setupServer(port);
	} else {
		setupClient(port, ip);
	}
}

SocketManager::~SocketManager() {
	spdlog::get("socket_logger")->debug("SocketManager deconstruction started");
	{
		std::lock_guard<std::mutex> lock(mtx);
		stopThread = true;
	}
	spdlog::get("socket_logger")->debug("set stopThread");
	cv.notify_all();
	spdlog::get("socket_logger")->debug("notified all");
	for (auto &thread : client_threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}
	spdlog::get("socket_logger")->debug("joinded all threads");
	for (int client_socket : client_sockets) {
		close(client_socket);
	}
	close(server_fd);
	spdlog::get("socket_logger")->debug("closed all sockets");

	spdlog::get("socket_logger")->debug("waiting for server thread");
	if (server_thread.joinable()) {
		server_thread.join();
	}
	spdlog::get("socket_logger")->debug("SocketManager deconstructed finished");
}

void SocketManager::setupServer(int port) {
	int opt = 1;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0) {
		perror("listen");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	this->server_thread = std::thread(&SocketManager::serverThreadFunction, this);
}

void SocketManager::serverThreadFunction() {
	fd_set readfds;
	struct timeval timeout;

	while (!stopThread) {
		FD_ZERO(&readfds);
		FD_SET(server_fd, &readfds);

		// Set timeout to 1 second
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		int activity = select(server_fd + 1, &readfds, nullptr, nullptr, &timeout);

		if (activity < 0) {
			perror("select");
			close(server_fd);
			exit(EXIT_FAILURE);
		}

		if (activity == 0) {
			// Timeout occurred, check if we need to stop the thread
			continue;
		}

		acceptClient(readfds);
	}
}

void SocketManager::acceptClient(fd_set &readfds) {
	if (FD_ISSET(server_fd, &readfds)) {
		int client_socket;
		auto addrlen = sizeof(address);
		if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
			perror("accept");
			close(server_fd);
			exit(EXIT_FAILURE);
		}
		handleNewClient(client_socket);
	}
}

void SocketManager::handleNewClient(int client_socket) {
	spdlog::get("socket_logger")->debug("client got");
	{
		std::lock_guard<std::mutex> lock(mtx);
		client_sockets.push_back(client_socket);
	}
	client_threads.emplace_back(&SocketManager::run, this, client_socket);
	spdlog::get("socket_logger")->debug("Client received finished");
}

void SocketManager::setupClient(int port, std::string ip) {
	if ((socket_id = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(port);

	spdlog::get("socket_logger")->debug("connecting to : {}", ip);
	if (inet_pton(AF_INET, ip.c_str(), &address.sin_addr) <= 0) {
		perror("Invalid address/ Address not supported");
		exit(EXIT_FAILURE);
	}

	if (connect(socket_id, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("Connection Failed");
		exit(EXIT_FAILURE);
	}
	spdlog::get("socket_logger")->debug("Client setup finished");
	this->client_sockets.push_back(socket_id);
	this->server_thread = std::thread(&SocketManager::run, this, socket_id);
}

void SocketManager::run(int client_socket) {
	fd_set readfds;
	struct timeval timeout;

	while (true) {
		FD_ZERO(&readfds);
		FD_SET(client_socket, &readfds);

		// Set timeout to 1 second
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		int activity = select(client_socket + 1, &readfds, nullptr, nullptr, &timeout);

		if (activity < 0) {
			spdlog::get("socket_logger")->debug("select error");
			break;
		}

		if (activity == 0) {
			// Timeout occurred, check if we need to stop the thread
			if (stopThread) {
				return;
			}
			continue;
		}

		if (FD_ISSET(client_socket, &readfds)) {
			readFromClient(client_socket);
		}

		if (stopThread) {
			return;
		}
	}
}

void SocketManager::readFromClient(int client_socket) {
	char buffer[BUFFER_SIZE] = {0};

	spdlog::get("socket_logger")->debug("waiting for read");
	int valread = read(client_socket, buffer, BUFFER_SIZE);
	spdlog::get("socket_logger")->debug("read finished");

	std::lock_guard<std::mutex> lock(mtx);
	if (valread < 0) {
		spdlog::get("socket_logger")->debug("Problem while reading");
	} else if (valread == 0) {
		spdlog::get("socket_logger")->debug("Client disconnected");
		spdlog::get("socket_logger")->debug("All messages:");

		for (const auto &message : messages) {
			spdlog::get("socket_logger")->debug(message.message);
		}
		this->sendMessage("Someone disconnected");
		close(client_socket);
		return;
	}

	processClientMessage(buffer, valread, client_socket);
}

void SocketManager::processClientMessage(char *buffer, int valread, int client_socket) {
	// Split the buffer on null-termination sign
	char *start = buffer;
	while (start < buffer + valread) {
		char *end = strchr(start, '\0');
		if (end == nullptr) {
			end = buffer + valread;
		}
		std::string message(start, end);
		messages.push_back(IncomingMessage{client_socket, message});
		spdlog::get("socket_logger")->debug("Received: {}", message);

		start = end + 1;
	}
}

IncomingMessage SocketManager::popMessage() {
	std::lock_guard<std::mutex> lock(mtx);
	if (messages.empty()) {
		return IncomingMessage{-1, ""};
	}
	auto message = messages.back();
	messages.pop_back();
	return message;
}

void SocketManager::sendMessage(std::string message) {
	spdlog::get("socket_logger")->debug("message sending");
	char buffer[BUFFER_SIZE] = {0};
	strncpy(buffer, (message + "\0").c_str(), BUFFER_SIZE - 1);
	spdlog::get("socket_logger")->debug("message copied");

	for (int client_socket : client_sockets) {
		ssize_t bytes_sent = send(client_socket, buffer, strlen(buffer), 0);
		if (bytes_sent < 0) {
			perror("send");
		} else {
			spdlog::get("socket_logger")->debug("Echo message sent: {}", message);
		}
	}
}
