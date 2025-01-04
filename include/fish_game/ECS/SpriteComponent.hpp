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
	SDL_Texture *texture;
	SDL_Rect srcRect, dstRect;

	bool animated = false;
	int frames = 1;
	int speed = 100; // ms
	int currentFrame = 0;

	std::string id;

	// animation meta data
	const Animation fishSwim = Animation(0, 12, 100);
	const Animation fishIdle = Animation(1, 12, 100);
	const Animation fishDead = Animation(2, 1, 100);

	std::map<std::string, Animation> animations = {
	    {"fishSwim", fishSwim}, {"fishIdle", fishIdle}, {"fishDead", fishDead}};

  public:
	int animationIndex = 0;
	// std::map<const char *, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	// ~SpriteComponent() = default;

	SpriteComponent(std::string id);

	SpriteComponent(std::string id, bool isAnimated);

	~SpriteComponent();

	void setTexture(std::string id);

	void init() override;

	void update() override;

	void draw() override;

	void play(const char *animName);
};

} // namespace FishEngine