#pragma once

// #include "ECS/Components.hpp"
#include "AssetManager.hpp"
#include "fish_game/ECS/ComponentsGenerator.hpp"
#include "fish_game/Map.hpp"
#include "fish_game/NetworkHost.hpp"
#include "fish_game/Player.hpp"

#include <SDL2/SDL.h>
#include <mutex>
#include <stdio.h>
#include <string>
#include <thread>
#include <vector>

namespace FishEngine {

enum groupLabels : std::size_t;

class ServerGame {

  public:
	static ServerGame &getInstance() {
		static ServerGame instance;
		return instance;
	}

	SDL_Event getEvent() { return game_event; }

	void init(fs::path mapPath);

	void update();

	void serverLoop();

	void printManager();

	uint8_t createPlayer();

	uint8_t createPlayer(int id);

	std::pair<std::uint16_t, std::uint16_t> getPlayerSpawnpoint();

	uint8_t handleJoinRequests();

	void receivePlayerEvents();

	void sendGameState();

	void spawnWeapons();

	void spawnWeaponsAux(const std::pair<std::uint16_t, std::uint16_t> &spawnpoint,
	                     const std::vector<Entity *> &existingWeapons);

	void insertToEntityGroups(uint8_t id, groupLabels label);

	void removeEntity(uint8_t id) { entityGroups.erase(id); }

	bool running() const;

	void stop();

	bool checkCollisions(Entity *player);

	void printAllEntityIDs();

	inline static SDL_Renderer *renderer = nullptr;
	inline static SDL_Event game_event;
	static SDL_Rect camera;

	NetworkHost networkHost;
	Map *map;
	Manager manager;

  private:
	ServerGame();
	ServerGame(const ServerGame &) = delete;
	ServerGame &operator=(const ServerGame &) = delete;

	bool isRunning;
	SDL_Window *window;

	std::vector<Player> players;

	std::map<uint8_t, groupLabels> entityGroups;
	std::vector<std::pair<uint16_t, uint16_t>> playerSpawnpoints;

	const unsigned int updateRate_ms = 150;
	unsigned int nextUpdate = 0;

	// server thread attributes
	std::thread serverThread;
	std::mutex serverMutex;
	std::condition_variable serverCv;
};

} // namespace FishEngine
