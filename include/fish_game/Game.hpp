#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>
#include <stdio.h>

namespace FishEngine
{

    class Game
    {

    public:
        Game();
        ~Game();

        void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);

        void initCombat();

        void handleEvents();

        void update();

        void render();

        void clean();

        bool running();

    private:
        int cnt;
        bool isRunning;
        SDL_Window *window;
        SDL_Renderer *renderer;
    };
} // namespace Game

#endif // GAME_HPP