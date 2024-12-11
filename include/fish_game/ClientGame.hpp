#pragma once

#include "AssetManager.hpp"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <vector>

namespace FishEngine {

class ClientGame {

  public:
	ClientGame(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);

	~ClientGame();

	SDL_Event getEvent() { return game_event; }

	void init(fs::path mp, int numPlayers, bool combat);

	void handleEvents();

	void update();

	void render();

	void spawnWeapons();

	void showIP(SDL_Texture *mTexture, int width, int height);

	uint8_t updateMainMenu();

	std::string joinInterface();

	void sendJoinRequest(std::string ip);

	void receiveGameState();

	bool hasStarted();

	bool running();

	void stop();

	void zoomIn();

	Manager *getManager();

	static SDL_Renderer *renderer;
	static SDL_Event game_event;
	static SDL_Rect camera;
	static AssetManager *assets;

	enum groupLabels : std::size_t {
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders,
		groupProjectiles,
		groupWeapons
	};

  private:
	uint8_t playerID;

	fs::path mapPath;
	int numPlayers;
	bool isRunning;
	bool started;

	SDL_Window *window;
	bool windowed = true;

	std::vector<Entity *> players;
	Entity *ownPlayer;
	std::map<uint8_t, ClientGame::groupLabels> entityIDs;
};

} // namespace FishEngine
