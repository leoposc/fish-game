#pragma once

#include "../Vector2D.hpp"
#include "ECS.hpp"

#include <cereal/types/polymorphic.hpp>

namespace FishEngine {

class TransformComponent : public Component {

	Vector2D velocity{0, 0};
	Vector2D position{0, 0};

	int speed = 3;
	int height = 16;
	int width = 16;
	float scale = 1;

	bool blocked = false;

  public:
	template <class Archive>
	void serialize(Archive &ar) {
		ar(speed, position, velocity, faceRight);
	}

	bool faceRight = false; // public because cereal's ar function called inside Server/ client game

	TransformComponent() : position(0, 0) {}
	TransformComponent(int x, int y) : position(x, y) {}
	TransformComponent(int sc) : scale(sc), position(0, 0) {}
	TransformComponent(int x, int y, int h, int w, float sc) : position(x, y), height(h), width(w), scale(sc) {}

	int getX() { return position.getX(); }
	int getY() { return position.getY(); }

	Vector2D getPosition() { return position; }

	void setPosition(const Vector2D pos) { position = pos; }

	Vector2D getVelocity() { return velocity; }

	void setVelocity(const Vector2D vel) { velocity = vel; }

	void init() override;

	void print() override;

	void update() override;

	bool isFacingRight() const { return faceRight; }

	void setFaceRight(const bool faceRight) { this->faceRight = faceRight; }

	float getScale() const { return scale; }

	int getWidth() const { return width; }

	void setWidth(const int w) { width = w; }

	int getHeight() const { return height; }

	void setHeight(const int h) { height = h; }
};

} // namespace FishEngine
