#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <SDL2/SDL.h>

class Object
{

public:
    Object(const char *texturePath, SDL_Renderer *renderer, int x, int y);
    ~Object();

    void update();
    void render();

private:
    int xpos;
    int ypos;

    SDL_Texture *texture;
    SDL_Rect srcRect, dstRect;
    SDL_Renderer *renderer;
};

#endif // OBJECT_HPP