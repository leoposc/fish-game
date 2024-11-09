#pragma once

#include "ECS.hpp"
#include "MoveComponent.hpp"
#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"
#include "ColliderComponent.hpp"
#include "KeyboardController.hpp"

namespace FishEngine {
class ComponentsGenerator {
    public:
        static void forPlayer(Entity &player, std::uint16_t x, std::uint16_t y) {
    	    player.addComponent<MoveComponent>(x, y, 45, 60, 1.0);
	        player.addComponent<SpriteComponent>("fish", false);
	        player.addComponent<ColliderComponent>("player", x, y, 45, 60);
            player.addComponent<KeyboardController>();
		    player.addGroup(Game::groupLabels::groupPlayers);
        }

        static void forProjectile(Entity &projectile, std::uint16_t x, std::uint16_t) {}
    };

    
}