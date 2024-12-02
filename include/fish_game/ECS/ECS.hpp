#pragma once

#include "../Vector2D.hpp"

#include <SDL2/SDL.h>
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cereal/archives/json.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <iostream>
#include <memory>
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

// CEREAL_REGISTER_TYPE(ColliderComponent)
// CEREAL_REGISTER_TYPE(WearableComponent)
// CEREAL_REGISTER_TYPE(EquipmentComponent)
// CEREAL_REGISTER_TYPE(EventHandlerComponent)

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID generateComponentID() {
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept {
	static ComponentID typeID = generateComponentID();
	return typeID;
}

constexpr std::size_t maxGroups = 32;
constexpr std::size_t maxComponents = 32;
using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component *, maxComponents>;

class Component {
  public:
	Entity *entity;

	template <class Archive>
	void serialize(Archive &ar) {}

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

	virtual ~Component() {}
};

class Entity {
	Manager &manager;

	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
	GroupBitSet groupBitSet;

  public:
	template <class Archive>
	void serialize(Archive &ar) {
		ar(active, getComponent<TransformComponent>(), getComponent<ColliderComponent>());

		if (hasComponent<EventHandlerComponent>()) {
			ar(getComponent<EventHandlerComponent>());
		}
		if (hasComponent<WearableComponent>()) {
			ar(getComponent<WearableComponent>());
		}
		if (hasComponent<EquipmentComponent>()) {
			ar(getComponent<EquipmentComponent>());
		}
	}

	Entity() : manager(manager) {} //
	Entity(Manager &man) : manager(man) {}

	Manager *getManager() { return &manager; }

	void addGroup(Group group);

	bool checkEmpty() { return components.empty(); }

	// template <class Archive>
	// void serialize(Archive &ar) {
	// 	ar(active, components, componentArray, componentBitSet, groupBitSet);
	// }

	/**
	 * @brief: update all components which belong to a instance of an entity
	 * @details: The order of updating components is important. It is defined by the order
	 * of adding components to an entity. The very first component added is updated first.
	 * The ComponentID is NOT used to determine the order of updating components.
	 */
	void update() {
		for (auto &c : components) {
			// std::cout << "ECS - Updating component" << std::endl;
			c->update();
		}
	}

	/**
	 * @brief: draw all components which belong to a instance of an entity
	 * @details: The order of drawing components is important. It is defined by the order
	 * of adding components to an entity. The very first component added is updated first.
	 * The ComponentID is NOT used to determine the order of updating components.
	 */
	void draw() {
		for (auto &c : components)
			c->draw();
	}

	bool isActive() const { return active; }
	void destroy() { active = false; }

	template <typename T>
	bool hasComponent() const {
		return componentBitSet[getComponentTypeID<T>()];
	}

	bool hasGroup(Group group) const { return groupBitSet[group]; }

	void delGroup(Group group) { groupBitSet[group] = false; }

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
};

class Manager {
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity *>, maxGroups> groupedEntities;

  public:
	template <class Archive>
	void save(Archive &ar) const {
		std::cout << "size of entites vector: " << entities.size() << std::endl;
		ar(entities);
	}

	template <class Archive>
	void load(Archive &ar) {
		ar(entities);
	}

	void update() {
		for (auto &e : entities) {
			// std::cout << "ECS - Updating entities" << std::endl;
			e->update();
		}
	}

	void draw() {
		for (auto &e : entities)
			e->draw();
	}

	void destroyEntities() {
		for (auto &e : entities)
			e->destroy();
		refresh();
		assert(checkEmpty());
	}

	bool checkEmpty() {
		for (auto &e : entities) {
			if (!e->checkEmpty()) {
				return false;
			}
		}
		return entities.empty();
	}

	void refresh() {
		for (auto i(0u); i < maxGroups; i++) {
			auto &v(groupedEntities[i]);
			v.erase(std::remove_if(std::begin(v), std::end(v),
			                       [i](Entity *entity) { return !entity->isActive() || !entity->hasGroup(i); }),
			        std::end(v));
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
		                              [](const std::unique_ptr<Entity> &mEntity) { return !mEntity->isActive(); }),
		               std::end(entities));
	}

	void addToGroup(Entity *entity, Group group) { groupedEntities[group].emplace_back(entity); }

	std::vector<Entity *> &getGroup(Group group) { return groupedEntities[group]; }

	Entity &addEntity() {
		// std::cout << "Adding entity" << std::endl;
		Entity *e = new Entity(*this);
		// std::cout << "Entity created" << std::endl;
		std::unique_ptr<Entity> uPtr(e);
		// std::cout << "Unique pointer created" << std::endl;
		entities.emplace_back(std::move(uPtr));
		// std::cout << "Entity moved" << std::endl;
		return *e;
	}
};

} // namespace FishEngine

// ================ CEREAL HELPER FUNCTIONS ================

template <class Archive>
void serialize(Archive &ar, SDL_Rect &rect) {
	ar(CEREAL_NVP(rect.x), CEREAL_NVP(rect.y), CEREAL_NVP(rect.w), CEREAL_NVP(rect.h));
}

template <class Archive>
void serialize(Archive &ar, FishEngine::Vector2D &vec) {
	ar(CEREAL_NVP(vec.getX()), CEREAL_NVP(vec.getY()));
}

// id synchronizaiton? maybe not needed if manager is always kept in synch, with cereal we can only update the changing
// variables
//
// if i created the object it might not match with the id of the host should i just prepend my username to all ids, and
// strip it when upacking? What if i get a whole new object? How do i match that?
//
// Other option: get all components and entities from server when joining -> all ids would auto match
//
//
// protocol: array of messages; if update: - update
//                              if new:    - generate Component, needed arguemnts given