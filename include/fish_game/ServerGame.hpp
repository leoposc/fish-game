#pragma once

// #include "ECS/Components.hpp"
#include "AssetManager.hpp"
#include "fish_game/NetworkHost.hpp"
#include "fish_game/Player.hpp"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <vector>

// class AssetManager;
class ColliderComponent;

namespace FishEngine {

// class AssetManager;
// class ColliderComponent;

class ServerGame {

  public:
	static ServerGame &getInstance() {
		static ServerGame instance;
		return instance;
	}

	SDL_Event getEvent() { return game_event; }

	void init(fs::path mapPath, int numPlayer);

	void handleEvents();

	void update();

	void printManager();

	uint8_t createPlayer();

	uint8_t handleJoinRequests();

	void updatePlayerEvent();

	void startGame();

	void sendGameState();

	bool running();

	void stop();

	static bool checkCollisions(Entity *player);

	static SDL_Renderer *renderer;
	static SDL_Event game_event;
	static SDL_Rect camera;
	static AssetManager *assets;

	enum groupLabels : std::size_t { groupMap, groupPlayers, groupEnemies, groupColliders, groupProjectiles };

	NetworkHost networkHost;

  private:
	ServerGame();
	~ServerGame();
	ServerGame(const ServerGame &) = delete;
	ServerGame &operator=(const ServerGame &) = delete;

	// fs::path mapPath;
	// bool started;

	bool isRunning;
	SDL_Window *window;

	std::vector<Player> players;

	std::map<uint8_t, groupLabels> entityGroups;
};

} // namespace FishEngine
