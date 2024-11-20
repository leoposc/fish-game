#ifndef GAME_INPUT_EVENTS_HPP
#define GAME_INPUT_EVENTS_HPP

#include <stdexcept>
#include <string>

class InputEvent {
  public:
	enum class Event {
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_UP,
		MOVE_DOWN,
		SHOOT,
	};

	// Serialize Event to string
	static std::string serialize(Event event);

	// Deserialize string to Event
	static Event deserialize(const std::string &str);
};

#endif // GAME_INPUT_EVENTS_HPP
