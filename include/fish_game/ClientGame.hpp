#pragma once

#include "AssetManager.hpp"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>

namespace FishEngine {

class ClientGame {

  public:
	ClientGame();
	~ClientGame();

	SDL_Event getEvent() { return game_event; }

	void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen, int numPlayers);

	void handleEvents();

	void update();

	void zoomIn();

	void render();

	void clean();

	bool running();

	void stop();

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
	bool isRunning;
	SDL_Window *window;
	bool windowed = true;

	std::vector<Entity *> players;
};

} // namespace FishEngine