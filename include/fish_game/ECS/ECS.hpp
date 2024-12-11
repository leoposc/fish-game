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
	int16_t id;
	bool active = true;
	std::vector<std::shared_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
	GroupBitSet groupBitSet;

  public:
	template <class Archive>
	void serialize(Archive &ar) {
		ar(entityIDs);

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

	template <class Archive>
	void load(Archive &ar) {
		ar(CEREAL_NVP(id), CEREAL_NVP(active), CEREAL_NVP(componentBitSet), CEREAL_NVP(groupBitSet));
	}

	// template <class Archive>
	// void save(Archive &ar) const {
	// 	ar(id, active, groupBitSet);
	// 	std::cout << "ECS - Serializing entity - groupbitset " << groupBitSet << std::endl;
	// 	ar(getComponent<TransformComponent>(), getComponent<ColliderComponent>());

	// 	if (hasComponent<EventHandlerComponent>()) {
	// 		ar(getComponent<EventHandlerComponent>());
	// 	}
	// 	if (hasComponent<WearableComponent>()) {
	// 		ar(getComponent<WearableComponent>());
	// 	}
	// 	if (hasComponent<EquipmentComponent>()) {
	// 		ar(getComponent<EquipmentComponent>());
	// 	}
	// }

	// template <class Archive>
	// void load(Archive &ar) {
	// 	ar(id, active, groupBitSet);
	// 	while (true) {
	// 		std::string componentName;
	// 		ar(componentName);
	// 		if (componentName.empty()) {
	// 			break;
	// 		}
	// 		if (componentName == typeid(TransformComponent).name()) {
	// 			addComponent<TransformComponent>();
	// 			ar(getComponent<TransformComponent>());
	// 		} else if (componentName == typeid(ColliderComponent).name()) {
	// 			addComponent<ColliderComponent>();
	// 			ar(getComponent<ColliderComponent>());
	// 		} else if (componentName == typeid(EventHandlerComponent).name()) {
	// 			addComponent<EventHandlerComponent>();
	// 			ar(getComponent<EventHandlerComponent>());
	// 		} else if (componentName == typeid(WearableComponent).name()) {
	// 			addComponent<WearableComponent>();
	// 			ar(getComponent<WearableComponent>());
	// 		} else if (componentName == typeid(EquipmentComponent).name()) {
	// 			addComponent<EquipmentComponent>();
	// 			ar(getComponent<EquipmentComponent>());
	// 		}
	// 	}
	// }

	Entity() : manager(manager) {} //
	Entity(Manager &man) : manager(man) {}

	Manager *getManager() { return &manager; }

	void setID(int16_t p_id) { id = p_id; }

	uint8_t getID() { return id; }

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
			// spdlog::get("console")->debug("Component Type: {}", typeid(*c).name());
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
};

class Manager {
	std::map<uint8_t, std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity *>, maxGroups> groupedEntities;
	std::map<uint8_t, std::unique_ptr<Entity>> entityIDs;

  public:
	template <class Archive>
	void save(Archive &ar) const {
		ar(entities);
	}

	template <class Archive>
	void load(Archive &ar) {
		std::map<uint8_t, std::unique_ptr<Entity>> s_entities;
		std::cout << "Loading entities" << std::endl;

		ar(entities);
		std::cout << "Entities loaded" << std::endl;
		std::cout << "Entities size: " << s_entities.size() << std::endl;
		// for (const auto &entry : s_entities) {
		// 	uint8_t id = entry.first;
		// 	auto &entity = entry.second;
		// 	std::cout << "Entity type: " << typeid(*entity).name() << std::endl;

		// 	if (entityExists(id)) {
		// 		ar(entities[id]);
		// 	} else {
		// 		std::cout << "Entity with id " << static_cast<int>(id) << " does not exist" << std::endl;
		// 	}
		// }

		// uint8_t id;
		// Entity entity;
		// ar(id, entity);

		// if (entityExists(id)) {
		// 	ar(id, entities[id]);
		// } else {
		// 	std::cout << "Entity with id " << id << " does not exist" << std::endl;
		// }
	}

	std::map<uint8_t, std::unique_ptr<Entity>> &getEntities() { return entities; }

	bool entityExists(uint8_t id) { return entities.find(id) != entities.end(); }
	Entity &getEntity(uint8_t id) const { return entityIDs.find(id); }

	void update() {
		for (auto &e : entities) {
			// std::cout << "ECS - Updating entities" << std::endl;
			e.second->update();
		}
	}

	void draw() {
		for (auto &e : entities)
			e.second->draw();
	}

	void destroyEntities() {
		for (auto &e : entities)
			e.second->destroy();
		refresh();
		assert(checkEmpty());
	}

	bool checkEmpty() {
		for (auto &e : entities) {
			if (!e.second->checkEmpty()) {
				return false;
			}
		}
		return entities.empty();
	}

	void refresh() {
		// for (auto i(0u); i < maxGroups; i++) {
		// 	auto &v(groupedEntities[i]);
		// 	v.erase(std::remove_if(std::begin(v), std::end(v),
		// 	                       [i](Entity *entity) { return !entity->isActive() || !entity->hasGroup(i); }),
		// 	        std::end(v));
		// }

		// entities.erase(std::remove_if(std::begin(entities), std::end(entities),
		//                               [](const std::unique_ptr<Entity> &mEntity) { return !mEntity->isActive(); }),
		//                std::end(entities));
	}

	// void addToGroup(Entity *entity, Group group) {}
	void addToGroup(Entity *entity, Group group) { groupedEntities[group].emplace_back(entity); }

	std::vector<Entity *> &getGroup(Group group) { return groupedEntities[group]; }

	// Entity &addEntity() { return this->addEntity(std::numeric_limits<uint8_t>::max()); }

	Entity &addEntity() {
		// std::cout << "Adding entity" << std::endl;
		Entity *e = new Entity(*this);
		// e->setID(p_id);
		// std::cout << "Entity created" << std::endl;
		std::unique_ptr<Entity> uPtr(e);
		// std::cout << "Unique pointer created" << std::endl;
		uint8_t id = generateEntityID();
		entities.emplace(id, std::move(uPtr));
		// std::cout << "Entity moved" << std::endl;
		return *e;
	}
};

} // namespace FishEngine

// ================ CEREAL HELPER FUNCTIONS ================

// template <class Archive>
// void serialize(Archive &ar, SDL_Rect &rect) {
// 	ar((rect.x), CEREAL_NVP(rect.y), CEREAL_NVP(rect.w), CEREAL_NVP(rect.h));
// }

// template <class Archive>
// void serialize(Archive &ar, FishEngine::Vector2D &vec) {
// 	ar(CEREAL_NVP(vec.getX()), CEREAL_NVP(vec.getY()));
// }

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