#include "../include/fish_game/Game.hpp"
#include "../include/fish_game/TextureManager.hpp"
#include "../include/fish_game/Object.hpp"
#include "../include/fish_game/ECS.hpp"
#include "../include/fish_game/Components.hpp"

#include <SDL2/SDL.h>
#include <iostream>

namespace FishEngine
{
    Object *player;

    Manager manager;
    // manager.addEntity();
    // manager.addEntity();
    // auto &newPlayer(manager.addEntity());

    Game::Game() : cnt(0), isRunning(false)
    {
    }

    Game::~Game()
    {
    }

    void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
    {
        int flags = 0;
        if (fullscreen)
        {
            flags = SDL_WINDOW_FULLSCREEN;
        }
        if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
        {
            // log init
            window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
            if (window)
            {
                // log window creation
            }

            renderer = SDL_CreateRenderer(window, -1, 0);
            if (renderer)
            {
                // log renderer created
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }

            isRunning = true;
        }
        else
        {
            isRunning = false;
        }
    }

    void Game::initCombat()
    {
        // init players
        // TODO: load every player from a file and change color
        player = new Object("assets/player.png", renderer, 0, 0);
    }

    void Game::handleEvents()
    {
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
        }
    }

    void Game::update()
    {
        manager.update();
    }

    void Game::render()
    {
        SDL_RenderClear(renderer);
        // put stuff to render
        // player->render();
        SDL_RenderPresent(renderer);
    }

    void Game::clean()
    {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        // log clean
    }

    bool Game::running()
    {
        return isRunning;
    }

}
