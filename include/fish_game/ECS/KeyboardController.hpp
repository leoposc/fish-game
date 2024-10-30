#ifndef KEYBOARDCONTROLLER_HPP
#define KEYBOARDCONTROLLER_HPP

#include "Components.hpp"
// #include "TransformComponent.hpp"
#include "MoveComponent.hpp"
// #include "../Game.hpp"

#include <SDL2/SDL.h>

namespace FishEngine
{

    class KeyboardController : public Component
    {
        MoveComponent *move;

    public:
        void init() override
        {
            move = &entity->getComponent<MoveComponent>();
        }

        void update() override
        {
            {
                switch (Game::game_event.key.keysym.sym)
                {
                // upwards
                case SDLK_w:
                    move->up();
                    break;
                // downwards
                case SDLK_s:
                    move->down();
                    break;
                // left
                case SDLK_a:
                    move->left();
                    break;
                // right
                case SDLK_d:
                    move->right();
                    break;
                }
            }
        }
    };

} // namespace FishEngine

#endif // KEYBO