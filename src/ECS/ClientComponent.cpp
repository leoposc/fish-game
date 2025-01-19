#include "../../include/fish_game/ECS/ClientComponent.hpp"
#include "../../include/fish_game/Auxiliary.hpp"
#include "../../include/fish_game/ECS/TransformComponent.hpp"
#include "../../include/fish_game/MockServer.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>
#include <fstream>

#include "fish_game/ClientGame.hpp"
#include "fish_game/NetworkHost.hpp"
#include "spdlog/spdlog.h"

namespace FishEngine {

void ClientComponent::init() {
	spdlog::get("console")->debug("Client Component Initialized");

	transform = &entity->getComponent<TransformComponent>();
}

void ClientComponent::sendEvent(SDL_Event &event) {
	// serilize the event and send it to the server
	uint8_t id = ClientGame::getInstance().ownPlayerID;
	std::cout << "ID before sending Event: " << static_cast<int>(id) << std::endl;
	std::ostringstream os;
	cereal::BinaryOutputArchive archive(os);

	archive(id, event.key.keysym.sym, event.type);

	auto serializedString = os.str();

	auto networkClient = &FishEngine::ClientGame::getInstance().networkClient;
	networkClient->sendEvent(serializedString);
}

} // namespace FishEngine

CEREAL_REGISTER_TYPE(FishEngine::ClientComponent);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::ClientComponent)
