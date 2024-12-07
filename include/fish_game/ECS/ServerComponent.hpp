#pragma once

#include "ECS.hpp"

namespace FishEngine {

class TransformComponent;

class ServerComponent : public Component {

	TransformComponent *transform = nullptr;

	SDL_Event event;

  public:
	template <class Archive>
	void serialize(Archive &ar) {}

	ServerComponent() = default;
	~ServerComponent() = default;

	void init() override;

	void update() override;

	/**
	 * @brief: returns a pointer to the event, whic is used
	 * by the EventHandlerComponent to get the event
	 */
	SDL_Event *getEventPtr() { return &event; }

	/**
	 * @brief: get the latest event and store it in the component's
	 * private SDL_Event event
	 * @details: The event is stored in a file called event.json
	 */
	void getEvent();
};

} // namespace FishEngine