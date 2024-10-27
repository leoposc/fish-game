#ifndef GAME_HPP
#define GAME_HPP

// #include "ECS/ECS.hpp"
// #include "ECS/Components.hpp"
// #include "ECS/ColliderComponent.hpp"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>

namespace FishEngine
{

    class ColliderComponent;

    class Game
    {

    public:
        static SDL_Event game_event;
        static SDL_Renderer *renderer;
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

        static void addTile(int id, int x, int y);

    private:
        int cnt;
        bool isRunning;
        SDL_Window *window;
    };
} // namespace FishEngine

#endif // GAME_HPP