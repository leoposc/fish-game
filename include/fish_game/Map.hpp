#ifndef MAP_HPP
#define MAP_HPP

#include <SDL2/SDL.h>

namespace FishEngine
{
    class Map
    {
    public:
        Map(SDL_Renderer *renderer);
        ~Map();

        void loadMap();

        void drawMap();

    private:
        SDL_Rect src, dst;

        SDL_Texture *plattforms;
        SDL_Texture *water;
        SDL_Texture *trees;

        int map[32][60];
    };
} // namespace FishEngine

#endif // MAP_HPP