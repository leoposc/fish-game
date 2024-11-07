#include "../include/fish_game/ECS/ECS.hpp"

namespace FishEngine {

void Entity::addGroup(Group group) {
  groupBitSet[group] = true;
  manager.addToGroup(this, group);
}

} // namespace FishEngine