#pragma once

#include <SDL2/SDL.h>
#include <mutex>
#include <queue>

#include "Vector2D.hpp"

class MockServer {
  public:
	static MockServer &getInstance() {
		static MockServer instance;
		return instance;
	}

	void enqueueEvent(SDL_Event event) { eventQueue.push(event); }

	bool pollEvent(SDL_Event &event) {
		if (eventQueue.empty()) {
			return false;
		}
		event = eventQueue.front();
		eventQueue.pop();

		return true;
	}

	void enqueuePosition(FishEngine::Vector2D position) { positionQueue.push(position); }

	bool pollPosition(FishEngine::Vector2D &position) {
		if (positionQueue.empty()) {
			return false;
		}

		position = positionQueue.front();
		positionQueue.pop();

		return true;
	}

  private:
	MockServer() {};

	MockServer(const MockServer &) = delete;
	MockServer &operator=(const MockServer &) = delete;

	std::queue<SDL_Event> eventQueue;
	std::queue<FishEngine::Vector2D> positionQueue;
};
