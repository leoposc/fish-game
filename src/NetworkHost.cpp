#include "../include/fish_game/NetworkHost.hpp"
#include "spdlog/spdlog.h"
#include <iostream>
#include <tuple>

NetworkHost::NetworkHost() : stopThread(false), socket(SocketManager(8080, true)) {
	this->workerThread = std::thread(&NetworkHost::threadFunction, this);
	spdlog::get("console")->debug("Thread created \n");
}

NetworkHost::~NetworkHost() {
	spdlog::get("console")->debug("NetworkHost deconstructed \n");
	this->stopThread = true;
	this->cv.notify_all();
	this->workerThread.join();
	spdlog::get("console")->debug("NetworkHost deconstructed finished\n");
}

std::optional<InputEvent::Event> NetworkHost::getAction() {
	std::lock_guard<std::mutex> lock(mtx);
	if (this->elementQueue.empty()) {
		return std::nullopt;
	}
	auto element = this->elementQueue.front();
	this->elementQueue.pop();
	return std::get<1>(element);
}

void NetworkHost::updateState(const std::string &updatedState) {
	std::lock_guard<std::mutex> lock(mtx);
	this->state = updatedState;
	this->socket.sendMessage(updatedState);
}

void NetworkHost::threadFunction() {
	int counter = 0;
	while (true) {
		counter++;
		{
			std::lock_guard<std::mutex> lock(mtx);
			if (stopThread) {
				return;
			}

			auto message = this->socket.popMessage();
			if (message.message == "") {
				continue;
			}
			if (clients.find(message.client_id) == clients.end()) {
				// first time -> register user
				spdlog::get("console")->debug("in register branch\n");
				clients.insert(std::make_pair(message.client_id, message.message));
			} else {
				spdlog::get("console")->debug("received");
				this->elementQueue.push(
				    std::make_tuple(clients[message.client_id], InputEvent::deserialize(message.message)));
			}
		}
	}
}
