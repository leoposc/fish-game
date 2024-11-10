#include "../include/fish_game/SocketManager.hpp"

SocketManager::SocketManager(int port, bool host) : addrlen(sizeof(address))
{
	if (host) {
		setupServer(port);
	} else {
		setupClient(port);
	}
	std::cout << "SocketManager initialized\n";
}

SocketManager::~SocketManager()
{
	std::cout << "SocketManager deconstruction started\n";
	{
		std::lock_guard<std::mutex> lock(mtx);
		stopThread = true;
	}
	std::cout << "set stopThread\n";
	cv.notify_all();
	std::cout << "notified all\n";
	for (auto &thread : client_threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}
	std::cout << "joinded all threads\n";
	for (int client_socket : client_sockets) {
		close(client_socket);
	}
	close(server_fd);
	std::cout << "closed all sockets\n";

	std::cout << "waiting for server thread\n";
	server_thread.join();
	std::cout << "SocketManager deconstructed finished\n";
}

void SocketManager::setupServer(int port)
{
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
				std::cout << "client got\n";
				std::lock_guard<std::mutex> lock(mtx);
				client_sockets.push_back(client_socket);
			}
			client_threads.emplace_back(&SocketManager::run, this, client_socket);
			std::cout << "Client received finished\n";
		}
	});
}

void SocketManager::setupClient(int port)
{
	if ((socket_id = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(port);

	if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0) {
		perror("Invalid address/ Address not supported");
		exit(EXIT_FAILURE);
	}

	if (connect(socket_id, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("Connection Failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "Client setup finished\n";
	this->client_sockets.push_back(socket_id);
	this->server_thread = std::thread(&SocketManager::run, this, socket_id);
}

void SocketManager::run(int client_socket)
{
	while (true) {
		char buffer[BUFFER_SIZE] = {0};

		std::cout << "waiting for read\n";
		int valread = read(client_socket, buffer, BUFFER_SIZE);
		std::cout << "read finished\n";

		std::lock_guard<std::mutex> lock(mtx);
		if (valread < 0) {
			std::cout << "Problem while reading\n";
			exit(EXIT_FAILURE);
		} else if (valread == 0) {
			std::cout << "Client disconnected\n";
			std::cout << "All messages:\n";

			for (const auto &message : messages) {
				std::cout << message.message << std::endl;
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
			std::cout << "Received: " << message << std::endl;

			start = end + 1;
		}

		if (stopThread) {
			return;
		}
	}
}

IncomingMessage SocketManager::popMessage()
{
	std::lock_guard<std::mutex> lock(mtx);
	if (messages.empty()) {
		return IncomingMessage{-1, ""};
	}
	auto message = messages.back();
	messages.pop_back();
	return message;
}

void SocketManager::sendMessage(std::string message)
{
	std::cout << "message sending\n";
	char buffer[BUFFER_SIZE] = {0};
	strncpy(buffer, (message + "\0").c_str(), BUFFER_SIZE - 1);
	std::cout << "message copied\n";

	for (int client_socket : client_sockets) {
		ssize_t bytes_sent = send(client_socket, buffer, strlen(buffer), 0);
		if (bytes_sent < 0) {
			perror("send");
		} else {
			std::cout << "Echo message sent: " << message << std::endl;
		}
	}
}
