#pragma once

#include "ECS.hpp"
#include "TransformComponent.hpp"

#include <SDL2/SDL.h>
#include <filesystem>
#include <map>
#include <string>

namespace FishEngine {

struct Animation {
	int index;
	int frames;
	int speed;

	Animation(int i, int f, int s) : index(i), frames(f), speed(s) {}
	Animation() : index(0), frames(1), speed(100) {}
};

class SpriteComponent : public Component {
	TransformComponent *transform;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
	SDL_Texture *texture;
	SDL_Rect srcRect, dstRect;
	std::string id;

	// animation meta data
	const Animation fishIdle = Animation(0, 20, 100);
	const Animation fishSwim = Animation(1, 12, 100);
	const Animation fishDead = Animation(2, 1, 100);
	std::unordered_map<std::string, Animation> animations = {
	    {"fishSwim", fishSwim}, {"fishIdle", fishIdle}, {"fishDead", fishDead}};

	bool animated = false;
	int currentFrame = 0;
	int animationIndex = 0;
	int speed = 100;
	int frames = 1;

  public:
	SpriteComponent() = default;
	SpriteComponent(std::string id);
	SpriteComponent(std::string id, bool isAnimated);
	~SpriteComponent() = default;

	void setTexture(std::string id);

	void init() override;

	void update() override;

	void draw() override;

	void play(const char *animName);

	void flipSprite(bool flip) { spriteFlip = flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE; }
};

} // namespace FishEngine