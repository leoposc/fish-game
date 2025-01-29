#include "../include/fish_game/NetworkHost.hpp"
#include "cereal/external/base64.hpp"
#include "spdlog/spdlog.h"
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

NetworkHost::NetworkHost() : stopThread(false), socket(SocketManager()) {
	this->workerThread = std::thread(&NetworkHost::threadFunction, this);
	this->socket.init(8080, "", true);
	spdlog::get("network_logger")->info("Thread created \n");
}

NetworkHost::~NetworkHost() {
	spdlog::get("network_logger")->info("NetworkHost deconstructed \n");
	this->stopThread = true;
	this->cv.notify_all();
	this->workerThread.join();
	if (this->workerThread.joinable()) {
		this->workerThread.join();
	}
	spdlog::get("network_logger")->info("NetworkHost deconstructed finished\n");
}

std::optional<std::string> NetworkHost::getAction() {
	std::lock_guard<std::mutex> lock(mtx);
	if (this->elementQueue.empty()) {
		return std::nullopt;
	}
	auto element = this->elementQueue.front();
	this->elementQueue.pop();
	return cereal::base64::decode(std::get<1>(element));
}

void NetworkHost::updateState(const std::string &updatedState) {
	std::lock_guard<std::mutex> lock(mtx);
	std::string encodedState =
	    cereal::base64::encode(reinterpret_cast<const unsigned char *>(updatedState.c_str()), updatedState.length());
	this->state = encodedState;
	this->socket.sendMessage(UPDATE_PREFIX + this->state);
}

void NetworkHost::notifyJoin(std::string username, int client_id) {
	this->socket.sendMessage(JOIN_PREFIX + std::to_string(client_id) + "%" + username);
	for (const auto &client : clients) {
		spdlog::get("network_logger")->info("SERVER: Client ID: {}, Client Name: {}", client.first, client.second);
	}
}

void NetworkHost::threadFunction() {
	int counter = 0;
	while (!stopThread) {
		counter++;
		{
			std::lock_guard<std::mutex> lock(mtx);

			auto message = this->socket.popMessage();
			if (message.message == "") {
				continue;
			}
			if (clients.find(message.client_id) == clients.end()) {
				// first time -> register user
				spdlog::get("network_logger")->debug("SERVER: Player " + message.message + " joined the game");
				clients.insert(std::make_pair(message.client_id, message.message));
				this->notifyJoin(message.message, message.client_id);
			} else {
				this->elementQueue.push(std::make_tuple(clients[message.client_id], message.message));
			}
		}
	}
}

void NetworkHost::printEventQueue() {
	std::queue<std::tuple<std::string, std::string>> tempQueue = elementQueue;

	while (!tempQueue.empty()) {
		auto event = tempQueue.front();
		tempQueue.pop();
	}
}

std::vector<std::string> NetworkHost::getClients() {
	std::vector<std::string> clientList;
	for (const auto &client : this->clients) {
		clientList.push_back(client.second);
	}

	return clientList;
}
