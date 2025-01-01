#include "../include/fish_game/ECS/ECS.hpp"

namespace FishEngine {

void Entity::addGroup(Group group) {
	spdlog::get("console")->info("before bitset");
	manager.print();
	spdlog::get("console")->info("after print");
	groupBitSet[group] = true;
	spdlog::get("console")->info("after bitset");
	manager.addToGroup(this, group);
	spdlog::get("console")->info("after addToGroup");
}

} // namespace FishEngine
