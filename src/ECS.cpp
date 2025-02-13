#include "../include/fish_game/ECS/ECS.hpp"

namespace FishEngine {

// ------------------------ Entity ------------------------
Entity::Entity(Manager &p_manager, uint8_t p_id) : manager(p_manager), id(p_id) {}

void Entity::addGroup(const Group group) {
	groupBitSet[group] = true;
	manager.addToGroup(this, group);
}

Manager *Entity::getManager() {
	return &manager;
}

uint8_t Entity::getID() {
	return id;
}

bool Entity::checkEmpty() {
	return components.empty();
}
void Entity::update() {
	for (auto &c : components) {
		// spdlog::get("console")->debug("Component Type: {}", typeid(*c).name());
		c->update();
	}
}

void Entity::draw() {
	for (auto &c : components)
		c->draw();
}

bool Entity::isActive() const {
	return active;
}

void Entity::destroy() {
	active = false;
}

bool Entity::hasGroup(Group group) const {
	return groupBitSet[group];
}

void Entity::delGroup(Group group) {
	groupBitSet[group] = false;
}

// ------------------------ Manager -----------------------

std::map<const uint8_t, std::unique_ptr<Entity>> &Manager::getEntities() {
	return entities;
}

bool Manager::entityExists(uint8_t id) {
	return entities.find(id) != entities.end();
}

Entity &Manager::getEntity(uint8_t id) const {
	auto it = entities.find(id);
	if (it == entities.end()) {
		throw std::runtime_error("Entity not found");
	}
	assert(it->first == it->second->getID());
	return *it->second;
}

void Manager::update() {
	for (auto &e : entities) {
		e.second->update();
	}
}

void Manager::draw() {
	for (auto &e : entities)
		e.second->draw();
}

bool Manager::checkEmpty() {
	for (auto &e : entities) {
		if (!e.second->checkEmpty()) {
			return false;
		}
	}
	return entities.empty();
}

// void addToGroup(Entity *entity, Group group) {}
void Manager::addToGroup(Entity *entity, Group group) {
	groupedEntities[group].emplace_back(entity);
}

std::vector<Entity *> &Manager::getGroup(Group group) {
	return groupedEntities[group];
}

const std::vector<Entity *> &Manager::getGroup_c(Group group) const {
	return groupedEntities[group];
}

Entity &Manager::addEntity() {
	return this->addEntity(generateEntityID());
	spdlog::get("console")->info("Entity added. size now: {}", entities.size());
}

Entity &Manager::addEntity(const uint8_t id) {
	// Check if the entity with the given ID already exists
	auto it = entities.find(id);
	if (it != entities.end()) {
		// Remove the existing entity
		entities.erase(it);
		// spdlog::get("console")->info("Existing entity with ID {} removed", id);
	}

	// Create a new entity
	Entity *e = new Entity(*this, id);
	// spdlog::get("console")->info("Creating new entity with ID {}", id);

	// Add the new entity to the map
	std::unique_ptr<Entity> uPtr(e);
	entities.emplace(id, std::move(uPtr));
	// spdlog::get("console")->info("New entity with ID {} added", id);
	assert(id == e->getID());
	return *e;
}

void Manager::print() const {
	for (const auto &pair : entities) {
		const auto &entity = pair.second;
		entity->print();
	}
}

} // namespace FishEngine
