#include "../include/fish_game/NetworkClient.hpp"
#include "cereal/external/base64.hpp"
#include "spdlog/spdlog.h"
#include <string>
#include <tuple>

NetworkClient::NetworkClient() : socket(SocketManager()), sendQueue(std::queue<std::string>()) {}

void NetworkClient::init(const std::string hostIP, const std::string username) {
	this->hostIP = hostIP;
	this->username = username;
	// TODO: pass ip
	this->socket.init(8080, false);

	this->workerThread = std::thread(&NetworkClient::run, this);
}

NetworkClient::~NetworkClient() {
	this->workerThread.join();
}

void NetworkClient::run() {
	this->socket.sendMessage(this->username);
	while (true) {
		if (!this->sendQueue.empty()) {
			auto sendEvent = this->sendQueue.front();
			this->socket.sendMessage(sendEvent);
			this->sendQueue.pop();
		}

		// TODO:move this into the client gameloop to avoid race conditions
		this->handleReceive();
	}
}

std::string NetworkClient::getUpdate() {
	std::lock_guard<std::mutex> lock(this->mutex);
	this->hasUpdateVal = false;
	auto updatedGamestate = cereal::base64::decode(this->gameState);
	spdlog::get("console")->debug("NetworkClient: Got new gamestate: " + updatedGamestate);
	return updatedGamestate;
}

void NetworkClient::handleReceive() {
	std::lock_guard<std::mutex> lock(this->mutex);
	std::string raw_message = this->socket.popMessage().message;

	size_t pos = raw_message.find(':');
	std::string prefix = raw_message.substr(0, pos + 1);
	std::string message = raw_message.substr(pos + 1);

	if (message == "") {
		return;
	}

	if (prefix == JOIN_PREFIX) {
		size_t pos = raw_message.find(':');

		int id = atoi(raw_message.substr(0, pos).c_str());
		std::string username = raw_message.substr(pos + 1);
		spdlog::get("console")->debug("CLIENT: Player " + message + " joined the game");
		this->newUsers.push_back(std::make_tuple(message, id));
		std::queue<std::string> empty;
		std::swap(this->sendQueue, empty);
	}
	if (prefix == UPDATE_PREFIX) {
		spdlog::get("console")->debug("CLIENT: Got updated gameState" + message);
		this->gameState = message;
		this->hasUpdateVal = true;
		spdlog::get("console")->debug("SET hasupdateVal to true");
	}
}

void NetworkClient::sendEvent(const std::string event) {
	std::lock_guard<std::mutex> lock(this->mutex);

	this->sendQueue.push(
	    cereal::base64::encode(reinterpret_cast<const unsigned char *>(event.c_str()), event.length()));
}

bool NetworkClient::hasUpdate() {
	std::lock_guard<std::mutex> lock(this->mutex);
	return this->hasUpdateVal;
}
