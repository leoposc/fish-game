#include "../include/fish_game/Game.hpp"
#include "../include/fish_game/Collision.hpp"
#include "../include/fish_game/ECS/Components.hpp"
#include "../include/fish_game/Map.hpp"
#include "../include/fish_game/TextureManager.hpp"
#include "../include/fish_game/Vector2D.hpp"

#include <SDL2/SDL.h>
#include <iostream>

namespace FishEngine {

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::game_event;

Manager manager;
Map *map;

auto &player(manager.addEntity());
auto &wall(manager.addEntity());

enum groupLabels : std::size_t {
  groupMap,
  groupPlayers,
  groupEnemies,
  groupColliders,
};

Game::Game() : cnt(0), isRunning(false) {}

Game::~Game() {}

void Game::init(const char *title, int xpos, int ypos, int width, int height,
                bool fullscreen) {
  int flags = 0;
  if (fullscreen) {
    flags = SDL_WINDOW_FULLSCREEN;
  }
  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    // log init
    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (window) {
      // log window creation
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer) {
      // log renderer created
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }

    isRunning = true;
  } else {
    isRunning = false;
  }

  // etc implementation - temporary test code below
  map = new Map();
  map->loadMap("../../maps/TestP.tmj");
  map->drawMap();

  float xy;
  // player.addComponent<SpriteComponent>("assets/RedFish.png");
  // player.addComponent<KeyboardController>();

  // wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1);
  // wall.addComponent<SpriteComponent>("assets/dirt.png");
}

void Game::initCombat() {
  // init players
  // TODO: load every player from a file and change color
}

void Game::handleEvents() {
  SDL_PollEvent(&game_event);
  switch (game_event.type) {
  case SDL_QUIT:
    isRunning = false;
    break;
  default:
    break;
  }
}

void Game::update() {
  manager.refresh();
  manager.update();
}

void Game::render() {
  SDL_RenderClear(renderer);
  // put stuff to render
  map->drawMap();
  manager.draw();
  // player->render();
  SDL_RenderPresent(renderer);
}

void Game::addTile(int id, int x, int y) {
  // auto &tile(manager.addEntity());
  // tile.addComponent<TileComponent>(x, y, 32, 32, id);
}

void Game::clean() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  // log clean
}

bool Game::running() { return isRunning; }

} // namespace FishEngine
