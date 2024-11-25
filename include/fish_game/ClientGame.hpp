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

	void spawnWeapons();

	void init(int numPlayers);

	void handleEvents();

	void update();

	void zoomIn();

	void render();

	bool running();

	bool hasStarted();

	void stop();

	Manager getManager();

	bool joinGame(std::string ip);

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
	fs::path mapPath;
	int numPlayers;
	bool isRunning;
	bool started;

	SDL_Window *window;
	bool windowed = true;

	std::vector<Entity *> players;
};

} // namespace FishEngine
