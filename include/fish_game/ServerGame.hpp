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

class ServerGame {

  public:
	ServerGame();
	~ServerGame();

	SDL_Event getEvent() { return game_event; }

	void init(const char *title, int numPlayer);

	void handleEvents();

	void update();

	void render();

	void clean();

	bool running();

	void stop();

	void checkWaterCollisions();

	void checkPlattformCollisions();

	static bool checkCollisions(Entity *player);

	static SDL_Renderer *renderer;
	static SDL_Event game_event;
	static SDL_Rect camera;
	static AssetManager *assets;

	enum groupLabels : std::size_t { groupMap, groupPlayers, groupEnemies, groupColliders, groupProjectiles };

  private:
	bool isRunning;
	SDL_Window *window;

	std::vector<Entity *> players;
};

} // namespace FishEngine