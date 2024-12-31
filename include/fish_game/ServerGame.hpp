#pragma once

// #include "ECS/Components.hpp"
#include "AssetManager.hpp"
#include "fish_game/NetworkHost.hpp"

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
		static ServerGame instance; // Guaranteed to be destroyed and instantiated on first use
		return instance;
	}

	SDL_Event getEvent() { return game_event; }

	void init(fs::path mapPath, int numPlayer);

	void handleEvents();

	void update();

	uint8_t createPlayer();

	uint8_t acceptJoinRequest();

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

	int numPlayers = 0;
	bool isRunning;
	SDL_Window *window;

	// std::vector<Entity *> players;

	std::unordered_map<uint8_t, std::string> playerIPs;
	std::unordered_map<uint8_t, Entity *> players;
	std::map<uint8_t, groupLabels> entityGroups;
};

} // namespace FishEngine
