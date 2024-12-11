#pragma once

// #include "ECS/Components.hpp"
#include "AssetManager.hpp"

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
	ServerGame();
	~ServerGame();

	SDL_Event getEvent() { return game_event; }

	void init(fs::path mapPath, int numPlayer);

	void handleEvents();

	void update();

	void acceptJoinRequest(std ::string ip);

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

  private:
	// fs::path mapPath;
	// bool started;

	int numPlayers = 0;
	bool isRunning;
	SDL_Window *window;

	std::vector<Entity *> players;

	std::vector<std::pair<std::string, std::uint8_t>> playerIDs;
	std::map<uint8_t, Entity *> playerEntities;
};

} // namespace FishEngine