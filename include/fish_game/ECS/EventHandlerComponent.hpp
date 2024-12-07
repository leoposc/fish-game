#pragma once

#include "../ServerGame.hpp"
#include "ECS.hpp"

#include <SDL2/SDL.h>

namespace FishEngine {

class MoveComponent;
class EquipmentComponent;
class ClientComponent;
class ServerComponent;

class EventHandlerComponent : public Component {
	bool isServer = false;

	SDL_Event *event_ptr;

  public:
	template <class Archive>
	void serialize(Archive &ar) {}

	MoveComponent *move;
	EquipmentComponent *equip;
	ClientComponent *clientComponent = nullptr;
	ServerComponent *serverComponent = nullptr;

	EventHandlerComponent() = default;
	~EventHandlerComponent() = default;

	EventHandlerComponent(bool isServer) : isServer(isServer){};

	/**
	 * @brief: initialize pointer to the move, equipment and client/server component
	 */
	void init() override;

	/**
	 * @brief: update the component
	 * @depends: on ClientComponent, since it sends the event to the server if initialized
	 * as a client
	 */
	void update() override;
};

} // namespace FishEngine