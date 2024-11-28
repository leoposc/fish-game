#pragma once

#include "ClientComponent.hpp"
#include "ColliderComponent.hpp"
#include "ECS.hpp"
#include "EquipmentComponent.hpp"
#include "EventHandlerComponent.hpp"
#include "GravityComponent.hpp"
#include "MoveComponent.hpp"
#include "ServerComponent.hpp"
#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"
#include "WearableComponent.hpp"

namespace FishEngine {

namespace ClientGenerator {

void forPlayer(Entity &player, std::uint16_t x, std::uint16_t y);

void forEnemy(Entity &enemy, std::uint16_t x, std::uint16_t y);

void forWeapon(Entity &weapon, std::uint16_t x, std::uint16_t y);

void forProjectile(Entity &projectile, std::uint16_t x, std::uint16_t);

} // namespace ClientGenerator

namespace ServerGenerator {

void forPlayer(Entity &player, std::uint16_t x, std::uint16_t y);

void forProjectile(Entity &projectile, std::uint16_t x, std::uint16_t);

} // namespace ServerGenerator

} // namespace FishEngine