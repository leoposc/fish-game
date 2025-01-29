#pragma once

#include "AssetManager.hpp"
#include "Map.hpp"
#include "NetworkClient.hpp"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <vector>

namespace FishEngine {

class ClientGame {

  public:
	static ClientGame &getInstance() {
		static ClientGame instance; // Guaranteed to be destroyed and instantiated on first use
		return instance;
	}

	SDL_Event getEvent() const { return game_event; }

	void init(fs::path mp, bool combat);

	void loadFishSprites();

	void handleEvents();

	void update();

	void render() const;

	void createOwnPlayer();

	void spawnWeaponsAux(const std::pair<std::uint16_t, std::uint16_t> &spawnpoint,
	                     const std::vector<Entity *> &existingWeapons);

	void spawnWeapons();

	void showIP(SDL_Texture *mTexture, int width, int height);

	uint8_t updateMainMenu() const;

	std::string joinInterface();

	void sendJoinRequest(std::string ip, std::string username);

	void receiveGameState();

	bool hasStarted();

	bool running() const;

	void stop();

	void zoomIn();

	void startLoadingBar();

	void renderLoadingBar();

	Manager *getManager();

	inline static SDL_Renderer *renderer = nullptr;
	inline static SDL_Event game_event;
	inline static AssetManager *assets;
	inline static SDL_Rect camera;

	NetworkClient networkClient;

	enum groupLabels : std::size_t {
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders,
		groupProjectiles,
		groupWeapons
	};

	uint8_t ownPlayerID;

  private:
	// Singelton
	ClientGame();
	ClientGame(const ClientGame &) = delete;
	ClientGame &operator=(const ClientGame &) = delete;
	~ClientGame();

	bool initialized;

	Manager clientManager;
	Entity *ownPlayer;

	Map *map = nullptr;
	fs::path mapPath;
	int numPlayers = 6;
	bool isRunning;
	bool started;
	bool connected = false;

	SDL_Window *window;
	bool windowed = true;

	std::unordered_map<uint8_t, Entity *> players;
	std::map<uint8_t, ClientGame::groupLabels> entityGroups;

	const char *title;
	int xpos;
	int ypos;
	int width;
	int height;
	bool fullscreen;

	// increment this for each new fish sprite and reset at init
	size_t fishSpriteID = 0;
	size_t progressUpdate = 0;
};

} // namespace FishEngine
