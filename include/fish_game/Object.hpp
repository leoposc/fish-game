#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <SDL2/SDL.h>

namespace FishEngine
{

    class Object
    {

    public:
        Object(const char *texturePath, int x, int y);
        ~Object() = default;

        void update();
        void render();

    private:
        int xpos;
        int ypos;

        SDL_Texture *texture;
        SDL_Rect srcRect, dstRect;
        SDL_Renderer *renderer;
    };

}

#endif // OBJECT_HPP