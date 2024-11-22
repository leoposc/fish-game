#pragma once

// #include "Components.hpp"
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

namespace FishEngine {

class Entity;
class Manager;
class Component;

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
	Entity(Manager &man) : manager(man) {}

	Manager *getManager() { return &manager; }

	void addGroup(Group group);

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