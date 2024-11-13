#pragma once

// #include "ECS/Components.hpp"
#include "AssetManager.hpp"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>

// class AssetManager;
class ColliderComponent;

namespace FishEngine {

// class AssetManager;
// class ColliderComponent;

class Game {

  public:
	// static std::vector<ColliderComponent *> colliders;

	Game();
	~Game();

	SDL_Event getEvent() { return game_event; }

	void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);

	void initCombat();

	void handleEvents();

	void update();

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
	int cnt;
	bool isRunning;
	SDL_Window *window;
	bool windowed = true;
};

} // namespace FishEngine