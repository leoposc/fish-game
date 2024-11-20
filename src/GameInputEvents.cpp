
#include "../include/fish_game/GameInputEvents.hpp"

std::string InputEvent::serialize(Event event)
{
	switch (event) {
	case Event::MOVE_LEFT:
		return "MOVE_LEFT";
	case Event::MOVE_RIGHT:
		return "MOVE_RIGHT";
	case Event::MOVE_UP:
		return "MOVE_UP";
	case Event::MOVE_DOWN:
		return "MOVE_DOWN";
	case Event::SHOOT:
		return "SHOOT";
	default:
		throw std::invalid_argument("Unknown InputEvent");
	}
}

InputEvent::Event InputEvent::deserialize(const std::string &str)
{
	if (str == "MOVE_LEFT")
		return Event::MOVE_LEFT;
	else if (str == "MOVE_RIGHT")
		return Event::MOVE_RIGHT;
	else if (str == "MOVE_UP")
		return Event::MOVE_UP;
	else if (str == "MOVE_DOWN")
		return Event::MOVE_DOWN;
	else if (str == "SHOOT")
		return Event::SHOOT;
	else
		throw std::invalid_argument("Unknown InputEvent string");
}
