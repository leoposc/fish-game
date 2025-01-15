#pragma once

#include "ECS.hpp"

namespace FishEngine {

class TransformComponent;

class ClientComponent : public Component {

	TransformComponent *transform = nullptr;

  public:
	// template <class Archive>
	// void serialize(Archive &ar) {
	// 	// ar(transform);
	// }

	ClientComponent() = default;
	virtual ~ClientComponent() = default;

	void init() override;

	/**
	 * @brief: send an event to the server
	 * @param event: the event to be sent
	 * @details: The event is serialized and sent to the server.
	 * The server will then deserialize the event and process it.
	 * If hosting the game, the event will still be sent to locahost.
	 * @depends: on EventHandlerComponent, which calls this function (twice).
	 */
	void sendEvent(SDL_Event &event);
};

} // namespace FishEngine
