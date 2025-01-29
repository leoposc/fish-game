#pragma once

// #include "ECS/Components.hpp"
#include "AssetManager.hpp"
#include "fish_game/Map.hpp"
#include "fish_game/NetworkHost.hpp"
#include "fish_game/Player.hpp"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <vector>

class ColliderComponent;

namespace FishEngine {

class ServerGame {

  public:
	static ServerGame &getInstance() {
		static ServerGame instance;
		return instance;
	}

	SDL_Event getEvent() { return game_event; }

	void init(fs::path mapPath);

	void handleEvents();

	void update();

	void printManager();

	uint8_t createPlayer();

	uint8_t createPlayer(int id);

	uint8_t handleJoinRequests();

	void updatePlayerEvent();

	void sendGameState();

	bool running() const;

	void stop();

	bool checkCollisions(Entity *player);

	inline static SDL_Renderer *renderer = nullptr;
	inline static SDL_Event game_event;
	static SDL_Rect camera;

	enum groupLabels : std::size_t { groupMap, groupPlayers, groupEnemies, groupColliders, groupProjectiles };

	NetworkHost networkHost;
	Map *serverMap;

  private:
	ServerGame();
	ServerGame(const ServerGame &) = delete;
	ServerGame &operator=(const ServerGame &) = delete;

	Manager manager;

	bool isRunning;
	SDL_Window *window;

	std::vector<Player> players;

	std::map<uint8_t, groupLabels> entityGroups;

	const unsigned int updateRate_ms = 150;
	unsigned int nextUpdate = 0;
};

} // namespace FishEngine
