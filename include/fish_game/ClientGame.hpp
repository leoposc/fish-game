#pragma once

#include "AssetManager.hpp"
#include "fish_game/NetworkClient.hpp"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <vector>

namespace FishEngine {

class ClientGame {

  public:
	ClientGame(const char *title, int xpos, int ypos);

	~ClientGame();

	SDL_Event getEvent() { return game_event; }

	void init(fs::path mp, int np, bool combat);

	void loadFishSprites();

	void handleEvents();

	void update();

	void render();

	void createOwnPlayer();

	void spawnWeapons();

	void showIP(SDL_Texture *mTexture, int width, int height);

	uint8_t updateMainMenu();

	std::string joinInterface();

	void sendJoinRequest(std::string ip, std::string username);

	void receiveGameState();

	bool hasStarted();

	bool running();

	void stop();

	void zoomIn();

	void startLoadingBar();

	void renderLoadingBar();

	Manager *getManager();

	static SDL_Renderer *renderer;
	static SDL_Event game_event;
	static SDL_Rect camera;
	static AssetManager *assets;

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
	Entity *ownPlayer;

	fs::path mapPath;
	int numPlayers;
	bool isRunning;
	bool started;

	SDL_Window *window;
	bool windowed = true;

	std::unordered_map<uint8_t, Entity *> players;
	std::map<uint8_t, ClientGame::groupLabels> entityGroups;

	// increment this for each new fish sprite and reset at init
	size_t fishSpriteID = 0;
	size_t progressUpdate = 0;
};

} // namespace FishEngine
