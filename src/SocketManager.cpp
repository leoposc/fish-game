#include "../include/fish_game/SocketManager.hpp"

#include "spdlog/spdlog.h"

SocketManager::SocketManager() : addrlen(sizeof(address)) {
	spdlog::get("console")->debug("SocketManager initialized");
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
	spdlog::get("console")->debug("SocketManager deconstruction started");
	{
		std::lock_guard<std::mutex> lock(mtx);
		stopThread = true;
	}
	spdlog::get("console")->debug("set stopThread");
	cv.notify_all();
	spdlog::get("console")->debug("notified all");
	for (auto &thread : client_threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}
	spdlog::get("console")->debug("joinded all threads");
	for (int client_socket : client_sockets) {
		close(client_socket);
	}
	close(server_fd);
	spdlog::get("console")->debug("closed all sockets");

	spdlog::get("console")->debug("waiting for server thread");
	server_thread.join();
	spdlog::get("console")->debug("SocketManager deconstructed finished");
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

	this->server_thread = std::thread([this]() {
		while (true) {
			int client_socket;
			if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
				perror("accept");
				close(server_fd);
				exit(EXIT_FAILURE);
			}
			{
				spdlog::get("console")->debug("client got");
				std::lock_guard<std::mutex> lock(mtx);
				client_sockets.push_back(client_socket);
			}
			client_threads.emplace_back(&SocketManager::run, this, client_socket);
			spdlog::get("console")->debug("Client received finished");
		}
	});
}

void SocketManager::setupClient(int port, std::string ip) {
	if ((socket_id = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(port);

	spdlog::get("console")->debug("connecting to : {}", ip);
	if (inet_pton(AF_INET, ip.c_str(), &address.sin_addr) <= 0) {
		perror("Invalid address/ Address not supported");
		exit(EXIT_FAILURE);
	}

	if (connect(socket_id, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("Connection Failed");
		exit(EXIT_FAILURE);
	}
	spdlog::get("console")->debug("Client setup finished");
	this->client_sockets.push_back(socket_id);
	this->server_thread = std::thread(&SocketManager::run, this, socket_id);
}

void SocketManager::run(int client_socket) {
	while (true) {
		char buffer[BUFFER_SIZE] = {0};

		spdlog::get("console")->debug("waiting for read");
		int valread = read(client_socket, buffer, BUFFER_SIZE);
		spdlog::get("console")->debug("read finished");

		std::lock_guard<std::mutex> lock(mtx);
		if (valread < 0) {
			spdlog::get("console")->debug("Problem while reading");
			// exit(EXIT_FAILURE);
		} else if (valread == 0) {
			spdlog::get("console")->debug("Client disconnected");
			spdlog::get("console")->debug("All messages:");

			for (const auto &message : messages) {
				spdlog::get("console")->debug(message.message);
			}
			this->sendMessage("Someone disconnected");
			close(client_socket);
			return;
		}

		// Split the buffer on null-termination sign
		char *start = buffer;
		while (start < buffer + valread) {
			char *end = strchr(start, '\0');
			if (end == nullptr) {
				end = buffer + valread;
			}
			std::string message(start, end);
			messages.push_back(IncomingMessage{client_socket, message});
			spdlog::get("console")->debug("Received: {}", message);

			start = end + 1;
		}

		if (stopThread) {
			return;
		}
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
	spdlog::get("console")->debug("message sending");
	char buffer[BUFFER_SIZE] = {0};
	strncpy(buffer, (message + "\0").c_str(), BUFFER_SIZE - 1);
	spdlog::get("console")->debug("message copied");

	for (int client_socket : client_sockets) {
		ssize_t bytes_sent = send(client_socket, buffer, strlen(buffer), 0);
		if (bytes_sent < 0) {
			perror("send");
		} else {
			spdlog::get("console")->debug("Echo message sent: {}", message);
		}
	}
}
