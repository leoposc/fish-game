#pragma once

#include "ECS.hpp"
// #include "TransformComponent.hpp"

namespace FishEngine {

class SpriteComponent;
class TransformComponent;
class GravityComponent;

class MoveComponent : public Component {

	TransformComponent *transform = nullptr;
	SpriteComponent *sprite = nullptr;
	GravityComponent *gravityC = nullptr;

	bool canJump = false;

	/* the component needs its own velocity attribute, since SDL_Event does not
	 * polls the key events every frame, which leads to the bug that the fish stops
	 * moving when it is inside the water, since the velocity is set to 0 every frame
	 * to stop gravity being applied inside the water
	 */

  public:
	float jumpForce = 0;
	Vector2D velocity{0, 0};
	bool inWater = false;

	MoveComponent() = default;
	~MoveComponent() = default;

	void init() override;

	void update() override;

	void up();

	void down();

	void left();

	void right();

	void stopX();

	void stopY();

	void collisionStop();

	bool isInWater() const;

	void setInWater(bool inWater);

	float getJumpForce() const;

	void setJumpForce(float jumpForce);

	Vector2D getVelocity() const;

	void setVelocity(const Vector2D velocity);
};

} // namespace FishEngine