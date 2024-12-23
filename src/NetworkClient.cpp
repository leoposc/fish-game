#include "../include/fish_game/NetworkClient.hpp"
#include "cereal/external/base64.hpp"
#include "spdlog/spdlog.h"
#include <string>
#include <tuple>

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
			this->socket.sendMessage(InputEvent::serialize(this->sendQueue.front()));
			this->sendQueue.pop();
		}

		// TODO:move this into the client gameloop to avoid race conditions
		this->handleReceive();
	}
}

std::string NetworkClient::getUpdate() {
	this->hasUpdateVal = false;
	auto updatedGamestate = cereal::base64::decode(this->gameState);
	spdlog::get("console")->debug("NetworkClient: Got new gamestate: " + updatedGamestate);
	return updatedGamestate;
}

void NetworkClient::handleReceive() {
	std::string raw_message = this->socket.popMessage().message;

	if (raw_message == "") {
		return;
	}

	size_t pos = raw_message.find(':');
	std::string prefix = raw_message.substr(0, pos + 1);
	std::string message = raw_message.substr(pos + 1);

	if (prefix == JOIN_PREFIX) {
		size_t pos = raw_message.find(':');

		int id = atoi(raw_message.substr(0, pos).c_str());
		std::string username = raw_message.substr(pos + 1);
		spdlog::get("console")->debug("CLIENT: Player " + message + " joined the game");
		this->newUsers.push_back(std::make_tuple(message, id));
	}
	if (prefix == UPDATE_PREFIX) {
		spdlog::get("console")->debug("CLIENT: Got updated gameState");
		this->hasUpdateVal = true;
		this->gameState = message;
	}
}

void NetworkClient::setEvent(const InputEvent::Event event) {
	std::lock_guard<std::mutex> lock(this->mutex);
	this->sendQueue.push(event);
}

bool NetworkClient::hasUpdate() {
	return this->hasUpdateVal;
}
