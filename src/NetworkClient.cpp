#include "../include/fish_game/NetworkClient.hpp"
#include "spdlog/spdlog.h"
#include <string>

NetworkClient::NetworkClient() : socket(SocketManager(8080, false)), sendQueue(std::queue<InputEvent::Event>()) {}

void NetworkClient::init(const std::string hostIP, const std::string username) {
	this->hostIP = hostIP;
	this->username = username;

	this->workerThread = std::thread(&NetworkClient::run, this);
}

NetworkClient::~NetworkClient() {
	this->workerThread.join();
}

void NetworkClient::run() {
	this->socket.sendMessage(this->username);
	while (true) {
		if (!this->sendQueue.empty()) {
			spdlog::get("console")->debug("mutex hello? dafuq\n");
			this->socket.sendMessage(InputEvent::serialize(this->sendQueue.front()));
			this->sendQueue.pop();
		}

		this->handleReceive();
	}
}

std::string NetworkClient::getUpdate() {
	return this->gameState;
}

void NetworkClient::handleReceive() {
	std::string raw_message = this->socket.popMessage().message;

	if (raw_message == "") {
		return;
	}

	size_t pos = raw_message.find(':');
	std::string prefix = (pos != std::string::npos) ? raw_message.substr(0, pos + 1) : raw_message;
	std::string message = (pos != std::string::npos) ? raw_message.substr(pos + 1) : "";

	spdlog::get("console")->debug("PREFIX:" + prefix);

	if (prefix == JOIN_PREFIX) {
		this->newUsers.push_back(message);
		spdlog::get("console")->debug("CLIENT: Player " + message + " joined the game");
	}
	if (prefix == UPDATE_PREFIX) {
		this->gameState = message;
	}
}

void NetworkClient::setEvent(const InputEvent::Event event) {
	std::lock_guard<std::mutex> lock(this->mutex);
	this->sendQueue.push(event);
}
