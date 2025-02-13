#pragma once

#include "../Vector2D.hpp"
#include "../include/cereal/types/bitset.hpp" // apparently not inside the cereal conan package

#include <SDL2/SDL.h>
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cereal/archives/json.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>

#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <vector>

namespace FishEngine {

class Entity;
class Manager;
class Component;
class ColliderComponent;
class WearableComponent;
class EquipmentComponent;
class EventHandlerComponent;
class TransformComponent;
class HealthComponent;

constexpr std::size_t maxGroups = 32;
constexpr std::size_t maxComponents = 32;
using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component *, maxComponents>;
using ComponentID = std::size_t;
using Group = std::size_t;

inline uint8_t generateEntityID() {
	static uint8_t id = 0;
	return id++;
}

inline ComponentID generateComponentID() {
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept {
	static ComponentID typeID = generateComponentID();
	return typeID;
}

class Component {
  public:
	Entity *entity;

	// template <class Archive>
	// void serialize(Archive &ar) {}

	virtual void init() {}
	/**
	 * @brief: update the component
	 * @details: If the virtual base class is not overridden, the default implementation
	 * will be called. Meaning that nothing will happen.
	 */
	virtual void update() {}
	/**
	 * @brief: draw the component
	 * @details: If the virtual base class is not overridden, the default implementation
	 * will be called. Meaning that nothing will happen.
	 */
	virtual void draw() {}

	virtual void print() {}

	virtual ~Component() {}
};

class Entity {
	Manager &manager;
	const uint8_t id;
	bool active = true;
	std::vector<std::shared_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
	GroupBitSet groupBitSet;

  public:
	template <class Archive>
	void serialize(Archive &ar) {
		ar(active, getComponent<TransformComponent>());

		// // ========== serialize optional components ==========
		if (hasComponent<WearableComponent>()) {
			ar(getComponent<WearableComponent>());
		}

		if (hasComponent<EquipmentComponent>()) {
			ar(getComponent<EquipmentComponent>());
		}

		if (hasComponent<HealthComponent>()) {
			ar(getComponent<HealthComponent>());
		}
	}

	Entity(Manager &p_manager, uint8_t p_id);

	Manager *getManager();

	uint8_t getID();

	void addGroup(const Group group);

	bool checkEmpty();

	/**
	 * @brief: update all components which belong to a instance of an entity
	 * @details: The order of updating components is important. It is defined by the order
	 * of adding components to an entity. The very first component added is updated first.
	 * The ComponentID is NOT used to determine the order of updating components.
	 */
	void update();

	/**
	 * @brief: draw all components which belong to a instance of an entity
	 * @details: The order of drawing components is important. It is defined by the order
	 * of adding components to an entity. The very first component added is updated first.
	 * The ComponentID is NOT used to determine the order of updating components.
	 */
	void draw();

	bool isActive() const;

	void destroy();

	template <typename T>
	bool hasComponent() const {
		return componentBitSet[getComponentTypeID<T>()];
	}

	bool hasGroup(Group group) const;

	void delGroup(Group group);

	template <typename T, typename... TArgs>
	T &addComponent(TArgs &&...mArgs) {
		return addComponent<T, -1>(std::forward<TArgs>(mArgs)...);
	}

	template <typename T, int N, typename... TArgs>
	T &addComponent(TArgs &&...mArgs) {
		// check if the component already exists
		assert(!hasComponent<T>());

		T *c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{c};
		if (N == -1) {
			components.emplace_back(std::move(uPtr));
		} else {
			components.emplace(components.begin() + N, std::move(uPtr));
		}

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template <typename T>
	T &getComponent() const {
		assert(hasComponent<T>());
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T *>(ptr);
	}

	// get a smart pointer to a component
	template <typename T>
	std::shared_ptr<T> getComponentSmartPtr() const {
		assert(hasComponent<T>());
		for (const auto &component : components) {
			// true if the component pointer is of type T
			if (std::dynamic_pointer_cast<T>(component)) {
				std::cout << component.use_count() << std::endl;
				return std::dynamic_pointer_cast<T>(component);
			}
		}
		return nullptr;
	}

	void print() const {
		// spdlog::get("network_logger")->info("Entity ID: {}", static_cast<int>(id));
		// spdlog::get("network_logger")->info("Active: {}", active ? "true" : "false");
		// spdlog::get("network_logger")->info("Manager Address: {}", &manager);
		// spdlog::get("network_logger")->info("Components: ");
		// for (const auto &component : components) {
		// 	spdlog::get("network_logger")->info("  - {}", typeid(*component).name());
		// 	component->print();
		// }
	}
};

class Manager {
	std::map<const uint8_t, std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity *>, maxGroups> groupedEntities;
	// std::map<uint8_t, std::unique_ptr<Entity>> entityIDs;

  public:
	std::map<const uint8_t, std::unique_ptr<Entity>> &getEntities();

	bool entityExists(uint8_t id);

	Entity &getEntity(uint8_t id) const;

	void update();

	void draw();

	template <class T>
	void destroyEntities() {
		for (auto &e : entities)
			e.second->destroy();
		refresh<T>();
		assert(checkEmpty());
	}

	bool checkEmpty();

	template <class T>
	void refresh() {
		for (auto i(0u); i < maxGroups; i++) {
			auto &v(groupedEntities[i]);
			v.erase(std::remove_if(std::begin(v), std::end(v),
			                       [i](Entity *entity) { return !entity->isActive() || !entity->hasGroup(i); }),
			        std::end(v));
		}

		// entities.erase(std::remove_if(std::begin(entities), std::end(entities),
		//                               [](const std::unique_ptr<Entity> &mEntity) { return !mEntity->isActive();
		//                               }),
		//                std::end(entities));

		for (auto it = entities.begin(); it != entities.end();) {
			if (!it->second->isActive()) {
				uint8_t id = it->second->getID();
				spdlog::get("console")->info("ECS refresh - Destroying entity with ID: {}", static_cast<int>(id));
				assert(id == it->first);
				T::getInstance().removeEntity(id);
				it = entities.erase(it);
			} else {
				++it;
			}
		}
	}

	// void addToGroup(Entity *entity, Group group) {}
	void addToGroup(Entity *entity, Group group);

	std::vector<Entity *> &getGroup(Group group);

	const std::vector<Entity *> &getGroup_c(Group group) const;

	Entity &addEntity();

	Entity &addEntity(const uint8_t id);

	void print() const;
};

} // namespace FishEngine
