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
        static void forPlayer(Entity &player) {
    	    player.addComponent<MoveComponent>(400, 240, 45, 60, 1.0);
	        player.addComponent<SpriteComponent>("fish", false);
	        player.addComponent<ColliderComponent>("player", 400, 240, 45, 60);
            player.addComponent<KeyboardController>();
		    player.addGroup(Game::groupLabels::groupPlayers);
        }
    };
}