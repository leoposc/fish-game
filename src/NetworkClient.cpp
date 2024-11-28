#include "../include/fish_game/NetworkClient.hpp"
#include "spdlog/spdlog.h"

NetworkClient::NetworkClient(const std::string hostIP, const std::string username)
    : socket(SocketManager(8080, false)), sendQueue(std::queue<InputEvent::Event>()) {
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
	}
}

std::string NetworkClient::getUpdate() {
	return this->socket.popMessage().message;
}

void NetworkClient::setEvent(const InputEvent::Event event) {
	std::lock_guard<std::mutex> lock(this->mutex);
	this->sendQueue.push(event);
}
