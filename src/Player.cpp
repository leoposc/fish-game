#include "../include/fish_game/Player.hpp"

Player::Player(int entity_id) : entity_id(entity_id) {
	this->sprite_id = generateSpriteID();
}
