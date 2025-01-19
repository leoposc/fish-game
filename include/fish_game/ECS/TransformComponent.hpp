#pragma once

#include "../Vector2D.hpp"
#include "ECS.hpp"

#include <cereal/types/polymorphic.hpp>

namespace FishEngine {

class TransformComponent : public Component {

  public:
	template <class Archive>
	void serialize(Archive &ar) {
		ar(speed, position, velocity, faceRight);
	}

	TransformComponent() : position(0, 0) {}

	TransformComponent(int x, int y) : position(x, y) {}

	TransformComponent(int sc) : scale(sc), position(0, 0) {}

	TransformComponent(int x, int y, int h, int w, float sc) : position(x, y), height(h), width(w), scale(sc) {}

	int getX() { return position.getX(); }
	int getY() { return position.getY(); }
	void setX(int x) { position.setX(x); }
	void setY(int y) { position.setY(y); }

	void setPos(int x, int y) {
		position.setX(x);
		position.setY(y);
	}

	void init() override;

	void update() override;

	Vector2D velocity{0, 0};
	Vector2D position{0, 0};

	int speed = 3;

	// TODO: read from file/ tileson
	int height = 16;
	int width = 16;

	float scale = 1;

	bool faceRight = false;
	bool blocked = false;
};

} // namespace FishEngine