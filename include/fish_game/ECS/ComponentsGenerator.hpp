#pragma once

#include "ClientComponent.hpp"
#include "ColliderComponent.hpp"
#include "ECS.hpp"
#include "EquipmentComponent.hpp"
#include "EventHandlerComponent.hpp"
#include "GravityComponent.hpp"
#include "HealthComponent.hpp"
#include "MoveComponent.hpp"
#include "ProjectileComponent.hpp"
#include "ServerComponent.hpp"
#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"
#include "WearableComponent.hpp"

#include <cstdint>
#include <utility>

namespace FishEngine {

namespace ClientGenerator {

void forPlayer(Entity &player, std::pair<std::uint16_t, std::uint16_t> const &pos);

void forEnemy(Entity &enemy, std::pair<std::uint16_t, std::uint16_t> const &pos);

void forWeapon(Entity &weapon, std::pair<std::uint16_t, std::uint16_t> const &pos);

void forProjectile(Entity &projectile, std::pair<std::uint16_t, std::uint16_t> const &pos);

} // namespace ClientGenerator

namespace ServerGenerator {

void forPlayer(Entity &player, std::pair<std::uint16_t, std::uint16_t> const &pos);

void forProjectile(Entity &projectile, std::pair<std::uint16_t, std::uint16_t> const &pos);

} // namespace ServerGenerator

} // namespace FishEngine